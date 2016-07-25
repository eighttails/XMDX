#include <QDebug>
#include <QBuffer>
#include <QProcessEnvironment>

#include "qmdxplayer.h"
#include "mdx2wav/gamdx/mxdrvg/mxdrvg.h"

typedef unsigned char u8;
typedef unsigned int u32;

extern void strcpy_cnv(char *dst, const char *src, int mode);
extern bool read_file(const char *name, int *fsize, u8 **fdata, int offset);
extern bool LoadMDX(const char *mdx_name, char *title, int title_len);

QMutex QMDXPlayer::mutex_;

QMDXPlayer::QMDXPlayer(QObject *parent)
    : QObject(parent)
    , audioOutput_(nullptr)
    , duration_(0.0)
    , maxSongDuration_(1500)
{
    //実行時に出る警告の抑止
    qRegisterMetaType<QAudio::State>();

    QAudioFormat format;
    format.setCodec("audio/pcm");
    format.setChannelCount(2);
    format.setSampleRate(44100);
    format.setSampleSize(16);
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::SignedInt);

    QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
    if (!info.isFormatSupported(format)) {
        qWarning()<<"raw audio format not supported by backend, cannot play audio.";
        format = info.nearestFormat(format);
    }

    audioOutput_ = new QAudioOutput(info, format, this);
    audioOutput_->setBufferSize(maxSongDuration_ * 44100 * 4);
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
    int SAMPLE_RATE = 44100;
    int filter_mode = 0;
    bool verbose = false;

    float max_song_duration = maxSongDuration_;
    int loop = loops;
    int fadeout = enableFadeout ? 1 : 0;
    char ym2151_type[8] = "fmgen";

    int AUDIO_BUF_SAMPLES = SAMPLE_RATE / 100; // 10ms

    // ファイル名がからの場合は環境変数からファイル名を取得(デバッグ用)
    const char *mdx_name = fileName == ""
            ? QProcessEnvironment::systemEnvironment().value("MDX_FILE_NAME").toLocal8Bit()
            : fileName.toLocal8Bit();
    if (mdx_name == 0 || *mdx_name == 0) {
        return false;
    }

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

    float song_duration = MXDRVG_MeasurePlayTime(loop, fadeout) / 1000.0f;
    // Warning: MXDRVG_MeasurePlayTime calls MXDRVG_End internaly,
    //          thus we need to call MXDRVG_PlayAt due to reset playing status.
    MXDRVG_PlayAt(0, loop, fadeout);

    if (verbose) {
        fprintf(stderr, "loop:%d fadeout:%d song_duration:%f\n", loop, fadeout, song_duration);
    }

    if (max_song_duration < song_duration) {
        song_duration = max_song_duration;
    }

    if (!renderWav) return true;

    short *audio_buf = new short [AUDIO_BUF_SAMPLES * 2];

    wavBuffer_.clear();
    wavBuffer_.reserve(SAMPLE_RATE * song_duration * 4);
    for (int i = 0; song_duration == 0.0f || 1.0f * i * AUDIO_BUF_SAMPLES / SAMPLE_RATE < song_duration; i++) {
        if (MXDRVG_GetTerminated()) {
            break;
        }

        float current = 1.0f * i * AUDIO_BUF_SAMPLES / SAMPLE_RATE;
        float fadevol = 1.0;
        if( song_duration - current < 10.0 ){
            fadevol = (song_duration - current) / 10;
        }

        int len = MXDRVG_GetPCM(audio_buf, AUDIO_BUF_SAMPLES);
        if (len <= 0) {
            break;
        }

        for (int j = 0; j < len * 2; j++){
            audio_buf[j] *= fadevol;
        }

        char* b = reinterpret_cast<char*>(audio_buf);
        for(size_t i = 0; i < len * 4; i++){
            wavBuffer_.push_back(b[i]);
        }
    }
    MXDRVG_End();

    delete []audio_buf;

    if (verbose) {
        fprintf(stderr, "completed.\n");
    }
    return true;
}

bool QMDXPlayer::startPlay()
{
    if(audioOutput_){
        if(audioOutput_->state() == QAudio::SuspendedState){
            audioOutput_->resume();
        } else {
            audioBuffer_ = audioOutput_->start();
            audioBuffer_->write(wavBuffer_);
        }
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

float QMDXPlayer::duration()
{
    return duration_;
}
