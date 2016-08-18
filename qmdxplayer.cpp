#include <QDebug>
#include <QBuffer>
#include <QProcessEnvironment>
#include <QTextCodec>
#include <QFileInfo>

#include "qmdxplayer.h"
#include "mdx2wav/gamdx/mxdrvg/mxdrvg.h"

typedef unsigned char u8;
typedef unsigned int u32;

extern void strcpy_cnv(char *dst, const char *src, int mode);
extern bool read_file(const char *name, int *fsize, u8 **fdata, int offset);
extern bool LoadMDX(const char *mdx_name, char *title, int title_len);

extern MXDRVG_EXPORT int MXDRVG_GetFadeoutStart(void);

constexpr size_t PLAY_SAMPLE_RATE = 44100;

QMutex QMDXPlayer::mutex_;

QMDXPlayer::QMDXPlayer(QObject *parent)
    : QObject(parent)
    , duration_(0.0)
    , maxSongDuration_(500)
    , wavIndex_(0)
{
    //実行時に出る警告の抑止
    qRegisterMetaType<QAudio::State>();

    QAudioFormat format;
    format.setCodec("audio/pcm");
    format.setChannelCount(2);
    format.setSampleRate(PLAY_SAMPLE_RATE);
    format.setSampleSize(16);
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::SignedInt);

    QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
    if (!info.isFormatSupported(format)) {
        qWarning()<<"raw audio format not supported by backend, cannot play audio.";
        format = info.nearestFormat(format);
    }

    audioOutput_ = new QAudioOutput(info, format, this);
    connect(audioOutput_.data(), &QAudioOutput::notify, this, &QMDXPlayer::writeAudioBuffer);
    connect(audioOutput_.data(), &QAudioOutput::notify, this, &QMDXPlayer::currentPositionChanged);
    connect(audioOutput_.data(), &QAudioOutput::stateChanged, this, &QMDXPlayer::stateChanged);
    audioOutput_->setBufferSize(PLAY_SAMPLE_RATE * 10);
    audioOutput_->setNotifyInterval(100);

    connect(this, &QMDXPlayer::songLoaded, this, &QMDXPlayer::titleChanged);
    connect(this, &QMDXPlayer::songLoaded, this, &QMDXPlayer::fileNameChanged);
    connect(this, &QMDXPlayer::songLoaded, this, &QMDXPlayer::durationChanged);
}

bool QMDXPlayer::loadSong(bool renderWav,
                          const QString &fileName,
                          const QString &pdxPath,
                          unsigned loops,
                          bool enableFadeout)
{
    QMutexLocker l(&mutex_);

    int MDX_BUF_SIZE = 256 * 1024;
    int PDX_BUF_SIZE = 1024 * 1024;
    int SAMPLE_RATE = PLAY_SAMPLE_RATE;
    int filter_mode = 0;
    bool verbose = false;

    float max_song_duration = maxSongDuration_;
    int loop = loops;
    int fadeout = enableFadeout ? 1 : 0;
    char ym2151_type[8] = "fmgen";

    int AUDIO_BUF_SAMPLES = SAMPLE_RATE / 100; // 10ms

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
        return false;
    }
    QTextCodec *sjis = QTextCodec::codecForName("Shift-JIS");
    title_ = sjis->toUnicode(title);

    float song_duration = MXDRVG_MeasurePlayTime(loop, fadeout) / 1000.0f;
    int doFadeout = MXDRVG_GetFadeoutStart();
    // Warning: MXDRVG_MeasurePlayTime calls MXDRVG_End internaly,
    //          thus we need to call MXDRVG_PlayAt due to reset playing status.
    MXDRVG_PlayAt(0, loop, fadeout);

    if (verbose) {
        fprintf(stderr, "loop:%d fadeout:%d song_duration:%f\n", loop, fadeout, song_duration);
    }

    if (max_song_duration < song_duration) {
        song_duration = max_song_duration;
    }
    duration_ = song_duration;

    if (!renderWav) return true;

    short *audio_buf = new short [AUDIO_BUF_SAMPLES * 2];

    wavIndex_ = 0;
    wavBuffer_.clear();
    wavBuffer_.reserve(SAMPLE_RATE * song_duration * 4);
    for (int i = 0; song_duration == 0.0f || 1.0f * i * AUDIO_BUF_SAMPLES / SAMPLE_RATE < song_duration; i++) {
        if (MXDRVG_GetTerminated()) {
            break;
        }

        float current = 1.0f * i * AUDIO_BUF_SAMPLES / SAMPLE_RATE;
        float fadevol = 1.0;
        if( doFadeout && song_duration - current < 10.0 ){
            fadevol = (song_duration - current) / 10.0;
        }

        int len = MXDRVG_GetPCM(audio_buf, AUDIO_BUF_SAMPLES);
        if (len <= 0) {
            break;
        }

        for (int j = 0; j < len * 2; j++){
            audio_buf[j] *= fadevol;
        }

        wavBuffer_.append(reinterpret_cast<char*>(audio_buf), len * 4);
    }
    MXDRVG_End();

    delete []audio_buf;

    if (verbose) {
        fprintf(stderr, "completed.\n");
    }
    emit songLoaded();
    return true;
}

bool QMDXPlayer::startPlay()
{
    if(audioOutput_){
        if(audioOutput_->state() == QAudio::SuspendedState){
            audioOutput_->resume();
        } else {
            wavIndex_ = 0;
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
    return float(audioOutput_->processedUSecs()) / 1000000;
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

void QMDXPlayer::writeAudioBuffer()
{
    qint64 wrote = audioBuffer_->write(&wavBuffer_.data()[wavIndex_], wavBuffer_.size() - wavIndex_);
    wavIndex_ += wrote;
}
