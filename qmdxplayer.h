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
    Q_PROPERTY(float duration READ duration NOTIFY playStarted)

public:
    explicit QMDXPlayer(QObject *parent = 0);

signals:
    void playStarted();
    void playFinished();
    void songLoaded();
    void titleChanged();
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
    float duration();

private:
    QString title_;
    float duration_;
    float maxSongDuration_;
    QByteArray wavBuffer_;

    QPointer<QIODevice> audioBuffer_;
    QPointer<QAudioOutput> audioOutput_;

    // 複数のインスタンスが同時にMXDEVを操作しないようにするための排他
    static QMutex mutex_;
};

#endif // QMDXPLAYER_H
