#include <signal.h>
#include <exception>

#include <QDebug>
#include <QBuffer>
#include <QProcessEnvironment>
#include <QTextCodec>
#include <QFileInfo>
#include <QTimer>

#include "qmdxplayer.h"
#include "mdx2wav/gamdx/mxdrvg/mxdrvg.h"

typedef unsigned char u8;
typedef unsigned int u32;

extern void strcpy_cnv(char *dst, const char *src, int mode);
extern bool read_file(const char *name, int *fsize, u8 **fdata, int offset);
extern bool LoadMDX(const char *mdx_name, char *title, int title_len);

extern MXDRVG_EXPORT int MXDRVG_GetFadeoutStart(void);

constexpr size_t PLAY_SAMPLE_RATE = 48000;
constexpr int CHANNELS = 2;
constexpr int SAMPLE_SIZE = 16;
constexpr int BYTES_PER_SAMPLE = SAMPLE_SIZE / 8 * CHANNELS;
constexpr int MDX_BUF_SIZE = 256 * 1024;
constexpr int PDX_BUF_SIZE = 1024 * 1024;
constexpr int SAMPLE_RATE = PLAY_SAMPLE_RATE;
constexpr int filter_mode = 0;
constexpr int AUDIO_BUF_SAMPLES = SAMPLE_RATE / 100; // 10ms

//曲の最後に1秒間の無音区間を入れる
//曲が短すぎる(1秒未満?)と終了を検知できないため、ダミーのデータを入れておく意味もある
constexpr float POST_GAP = 1.0f;

QMutex QMDXPlayer::mutex_(QMutex::Recursive);
std::shared_ptr<std::thread> QMDXPlayer::renderingThread_;
std::atomic_bool QMDXPlayer::quitRenderingThread_(false);

// MXDRG内部で発生したSIGSEGVを無理やりcatchしてクラッシュを回避する。
class MXDRVException : public std::exception
{
public:
	const char* what() const noexcept override
	{
		return "segfault in MXDRV";
	}
};

void onMXDRVSegFault(int param)
{
	throw MXDRVException();
}


QMDXPlayer::QMDXPlayer(QObject *parent)
	: QObject(parent)
	, isSongLoaded_(false)
	, duration_(0.0)
	, maxSongDuration_(20*60 - POST_GAP)
	, wavIndex_(0)
{
	//実行時に出る警告の抑止
	qRegisterMetaType<QAudio::State>();

	QAudioFormat format;
	format.setCodec("audio/pcm");
	format.setChannelCount(CHANNELS);
	format.setSampleRate(PLAY_SAMPLE_RATE);
	format.setSampleSize(SAMPLE_SIZE);
	format.setByteOrder(QAudioFormat::LittleEndian);
	format.setSampleType(QAudioFormat::SignedInt);

	QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
	if (!info.isFormatSupported(format)) {
		qWarning()<<"raw audio format not supported by backend, cannot play audio.";
		format = info.nearestFormat(format);
	}

	audioOutput_ = new QAudioOutput(info, format, this);
	QTimer* timer = new QTimer(this);
	timer->setInterval(50);
	timer->start();
	connect(timer, &QTimer::timeout, this, &QMDXPlayer::writeAudioBuffer);
	connect(audioOutput_.data(), &QAudioOutput::stateChanged, this, &QMDXPlayer::onStateChanged);
	audioOutput_->setBufferSize(PLAY_SAMPLE_RATE);

	connect(this, &QMDXPlayer::isSongLoadedChanged, [this]{emit titleChanged(title_);});
	connect(this, &QMDXPlayer::isSongLoadedChanged, [this]{emit fileNameChanged(fileName_);});
	connect(this, &QMDXPlayer::isSongLoadedChanged, [this]{emit durationChanged(duration_);});
	connect(this, &QMDXPlayer::isSongLoadedChanged, [this]{emit durationStringChanged(durationString());});
}

QMDXPlayer::~QMDXPlayer()
{
	terminateRenderingThread();
}

bool QMDXPlayer::loadSong(bool renderWav,
						  const QString &fileName,
						  const QString &pdxPath,
						  unsigned loops,
						  bool enableFadeout)
{
	// レンダリングスレッドが起こっていたら先に終了しておく
	terminateRenderingThread();
	{
		QMutexLocker l(&mutex_);

		title_.clear();
		fileName_.clear();
		duration_ = 0;
		setIsSongLoaded(false);

		float max_song_duration = maxSongDuration_;
		int loop = loops;
		int fadeout = enableFadeout ? 1 : 0;
		char ym2151_type[8] = "fmgen";

		// ファイル名がからの場合は環境変数からファイル名を取得(デバッグ用)
		const QByteArray openFileName = fileName.isEmpty()
				? QProcessEnvironment::systemEnvironment().value("MDX_FILE_NAME").toLocal8Bit()
				: fileName.toLocal8Bit();
		const char *mdx_name = openFileName.data();
		if (mdx_name == 0 || *mdx_name == 0) {
			return false;
		}
		fileName_ = QFileInfo(openFileName).fileName();

		if (0 == strcmp(ym2151_type, "fmgen")) {
		} else if (0 == strcmp(ym2151_type, "mame")) {
			MXDRVG_SetEmulationType(MXDRVG_YM2151TYPE_MAME);
		} else {
			fprintf(stderr, "Invalid ym2151 emulation type: %s.\n", ym2151_type);
			return false;
		}

		MXDRVG_Start(SAMPLE_RATE, filter_mode, MDX_BUF_SIZE, PDX_BUF_SIZE);
		MXDRVG_TotalVolume(256);

		char title[256];

		if (!LoadMDX(mdx_name, title, sizeof(title))) {
			MXDRVG_End();
			return false;
		}
		QTextCodec *sjis = QTextCodec::codecForName("Shift-JIS");
		title_ = sjis->toUnicode(title);

		float song_duration = MXDRVG_MeasurePlayTime(loop, fadeout) / 1000.0f;
		doFadeout_ = MXDRVG_GetFadeoutStart() ? true : false;
		// Warning: MXDRVG_MeasurePlayTime calls MXDRVG_End internaly,
		//          thus we need to call MXDRVG_PlayAt due to reset playing status.
		MXDRVG_PlayAt(0, loop, fadeout);

		if (max_song_duration < song_duration) {
			song_duration = max_song_duration;
		}
		duration_ = song_duration + POST_GAP;

		if (!renderWav){
			MXDRVG_End();
			return true;
		}

		// レンダリングを非同期で開始
		// MXDRVG_End()は別スレッドで呼び出す
		startRenderingThread();
	}
	setIsSongLoaded(true);
	return true;
}

bool QMDXPlayer::startPlay()
{
	if(audioOutput_){
		if(audioOutput_->state() == QAudio::SuspendedState){
			audioOutput_->resume();
		} else {
			audioBuffer_ = audioOutput_->start();
		}
		writeAudioBuffer();
	} else {
		return false;
	}
	return true;
}
bool QMDXPlayer::stopPlay()
{
	if(audioOutput_){
		audioOutput_->suspend();
	} else {
		return false;
	}
	return true;
}

bool QMDXPlayer::setCurrentPosition(float position)
{
	QMutexLocker l(&mutex_);
	// 秒数×サンプルレート×サンプルサイズがバファ上のインデックスとなる
	size_t index = position * SAMPLE_RATE * BYTES_PER_SAMPLE;
	// サンプルサイズの整数倍に切り詰める
	index -= index % BYTES_PER_SAMPLE;
	// 算出したインデックスがバッファサイズより大きい(レンダリングが完了してない)場合は抜ける
	if(index > wavBuffer_.size()) return false;

	wavIndex_ = index;
	emit currentPositionChanged(currentPosition());
	return true;
}

QString QMDXPlayer::title()
{
	return title_;
}

QString QMDXPlayer::fileName()
{
	return fileName_;
}

float QMDXPlayer::duration()
{
	return duration_;
}

QString QMDXPlayer::durationString()
{
	int d = static_cast<int>(duration());
	return QString("%1:%2")
			.arg(d / 60, 2, 10, QLatin1Char('0'))
			.arg(d % 60, 2, 10, QLatin1Char('0'));
}

float QMDXPlayer::currentPosition()
{
	return static_cast<float>(wavIndex_) / BYTES_PER_SAMPLE / PLAY_SAMPLE_RATE;
}

QString QMDXPlayer::currentPositionString()
{
	int p = static_cast<int>(currentPosition());
	return QString("%1:%2")
			.arg(p / 60, 2, 10, QLatin1Char('0'))
			.arg(p % 60, 2, 10, QLatin1Char('0'));
}

bool QMDXPlayer::isPlaying()
{
	return audioOutput_->state() == QAudio::ActiveState;
}

bool QMDXPlayer::isSongLoaded()
{
	return isSongLoaded_;
}

void QMDXPlayer::setIsSongLoaded(bool isSongLoaded)
{
	isSongLoaded_ = isSongLoaded;
	emit isSongLoadedChanged(isSongLoaded_);
}

void QMDXPlayer::startRenderingThread()
{
	if(renderingThread_){
		terminateRenderingThread();
	}
	renderingThread_.reset(new std::thread(&QMDXPlayer::renderingThread, this));
}

void QMDXPlayer::terminateRenderingThread()
{
	if(renderingThread_){
		quitRenderingThread_ = true;
		renderingThread_->join();
		renderingThread_.reset();
		quitRenderingThread_ = false;
	}
}

void QMDXPlayer::onStateChanged(QAudio::State state)
{
	bool isPlayFinished = false;
	{
		QMutexLocker l(&mutex_);
		// 曲が最後まで処理されていたら終了通知を出す
		if(state == QAudio::IdleState && wavIndex_ && wavIndex_ == wavBuffer_.size()){
			isPlayFinished = true;
		}
	}
	if(isPlayFinished) emit songPlayFinished();
	emit stateChanged();
}

void QMDXPlayer::renderingThread()
{
	try{
		// MXDRG内部で発生したSIGSEGVを無理やりcatchしてクラッシュを回避する。
		signal(SIGSEGV, onMXDRVSegFault);

		{
			QMutexLocker l(&mutex_);
			wavIndex_ = 0;
			wavBuffer_.clear();
			wavBuffer_.reserve(SAMPLE_RATE * duration_ * BYTES_PER_SAMPLE);
		}

		short audio_buf[AUDIO_BUF_SAMPLES * 2];
		for (int i = 0; duration_ == 0.0f || 1.0f * i * AUDIO_BUF_SAMPLES / SAMPLE_RATE < duration_; i++) {
			if (quitRenderingThread_ || MXDRVG_GetTerminated()) {
				break;
			}

			float current = 1.0f * i * AUDIO_BUF_SAMPLES / SAMPLE_RATE;
			float fadevol = 1.0;
			if( doFadeout_ && duration_ - current < 10.0 ){
				fadevol = (duration_ - current) / 10.0;
			}

			int len = MXDRVG_GetPCM(audio_buf, AUDIO_BUF_SAMPLES);
			if (len <= 0) {
				break;
			}

			for (int j = 0; j < len * 2; j++){
				audio_buf[j] *= fadevol;
			}

			QMutexLocker l(&mutex_);
			wavBuffer_.append(reinterpret_cast<char*>(audio_buf), len * BYTES_PER_SAMPLE);
		}

		{
			QMutexLocker l(&mutex_);
			QByteArray postGap(int(POST_GAP * SAMPLE_RATE * BYTES_PER_SAMPLE), 0);
			wavBuffer_.append(postGap);
		}
	} catch(const MXDRVException& e) {
		qDebug() << "exception caught:" << e.what();
		// 例外が発生したら強制的に再生を終了して次の曲へ
		emit songPlayFinished();
		emit stateChanged();
	} catch(const std::exception& e) {
		qDebug() << "exception caught:" << e.what();
	}catch(...) {
		qDebug() << "unknown exception caught.";
	}

	MXDRVG_End();
}

void QMDXPlayer::writeAudioBuffer()
{
	{
		QMutexLocker l(&mutex_);
		if(!audioBuffer_) return;
		qint64 wrote = audioBuffer_->write(&wavBuffer_.data()[wavIndex_], wavBuffer_.size() - wavIndex_);
		wavIndex_ += wrote;
	}
	emit currentPositionChanged(currentPosition());
}

