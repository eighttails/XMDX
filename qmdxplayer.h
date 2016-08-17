#ifndef QMDXPLAYER_H
#define QMDXPLAYER_H

#include <QObject>
#include <QPointer>
#include <QMutex>
#include <QAudioOutput>

class QMDXPlayer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title NOTIFY titleChanged)
    Q_PROPERTY(QString fileName READ fileName NOTIFY fileNameChanged)
    Q_PROPERTY(float duration READ duration NOTIFY songLoaded)
    Q_PROPERTY(bool isPlaying READ isPlaying NOTIFY stateChanged)

public:
    explicit QMDXPlayer(QObject *parent = 0);

signals:
    void stateChanged();
    void songLoaded();
    void titleChanged();
    void fileNameChanged();
    void durationChanged();

public slots:
    bool loadSong(bool renderWav,
                  const QString& fileName,
                  const QString& pdxPath,
                  unsigned loops,
                  bool enableFadeout);
    bool startPlay();
    bool stopPlay();

public:
    QString title();
    QString fileName();
    float duration();
    bool isPlaying();

private slots:
    void writeAudioBuffer();
private:
    // 曲のタイトル
    QString title_;

    // 曲のファイル名(表示用)
    QString fileName_;

    // 曲の長さ(秒)
    float duration_;
    float maxSongDuration_;

    // レンダリングしたWAVバッファ
    QByteArray wavBuffer_;
    // 現在再生中のバッファインデックス
    size_t wavIndex_;

    QPointer<QIODevice> audioBuffer_;
    QPointer<QAudioOutput> audioOutput_;

    // 複数のインスタンスが同時にMXDEVを操作しないようにするための排他
    static QMutex mutex_;
};

#endif // QMDXPLAYER_H
