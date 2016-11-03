#ifndef QMDXPLAYER_H
#define QMDXPLAYER_H

#include <thread>
#include <QObject>
#include <QPointer>
#include <QMutex>
#include <QAudioOutput>

class QMDXPlayer : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QString title READ title NOTIFY titleChanged)
	Q_PROPERTY(QString fileName READ fileName NOTIFY fileNameChanged)
	Q_PROPERTY(float duration READ duration NOTIFY durationChanged)
	Q_PROPERTY(QString durationString READ durationString NOTIFY durationChanged)
	Q_PROPERTY(float currentPosition READ currentPosition WRITE setCurrentPosition NOTIFY currentPositionChanged)
	Q_PROPERTY(QString currentPositionString READ currentPositionString NOTIFY currentPositionChanged)
	Q_PROPERTY(bool isPlaying READ isPlaying NOTIFY stateChanged)
	Q_PROPERTY(bool isSongLoaded READ isSongLoaded NOTIFY isSongLoadedChanged)


public:
	explicit QMDXPlayer(QObject *parent = 0);
	~QMDXPlayer() override;
signals:
	void stateChanged();
	void isSongLoadedChanged();
	void titleChanged();
	void fileNameChanged();
	void durationChanged();
	void currentPositionChanged();
	void songPlayFinished();

public slots:
	// 曲をロードする。
	// 演奏せずにファイル情報を取得するだけの場合はrenderWavをfalseにする。
	bool loadSong(bool renderWav,
				  const QString& fileName,
				  const QString& pdxPath,
				  unsigned loops,
				  bool enableFadeout);
	// 演奏開始
	bool startPlay();
	// 演奏中断
	bool stopPlay();
	// 曲中のシーク(引数は曲頭からの秒数)
	bool setCurrentPosition(float position);

public:
	// 曲名を取得
	QString title();
	// ファイル名を取得
	QString fileName();
	// 曲長(秒)を取得
	float duration();
	// 曲長(秒)をmm:ssフォーマットで取得
	QString durationString();
	// 演奏中の位置(秒)を取得
	float currentPosition();
	// 演奏中の位置(秒)をmm:ssフォーマットで取得
	QString currentPositionString();
	// 演奏中かどうか取得
	bool isPlaying();
	// 曲がロードされているかどうか取得
	bool isSongLoaded();

private slots:
	void writeAudioBuffer();
	void setIsSongLoaded(bool isSongLoaded);
	void startRenderingThread();
	void terminateRenderingThread();
	void onStateChanged(QAudio::State state);
private:
	// サウンドデータのレンダリングスレッド
	static std::atomic_bool quitRenderingThread_;
	static std::shared_ptr<std::thread> renderingThread_;
	void renderingThread();

	// 曲のタイトル
	QString title_;

	// 曲のファイル名(表示用)
	QString fileName_;

	// 曲がロードされているか
	bool isSongLoaded_;

	// 曲の長さ(秒)
	float duration_;
	float maxSongDuration_;

	// 曲がフェードアウトするかどうか
	bool doFadeout_;

	// レンダリングしたWAVバッファ
	QByteArray wavBuffer_;
	// 現在再生中のバッファインデックス
	std::atomic_size_t wavIndex_;

	QPointer<QIODevice> audioBuffer_;
	QPointer<QAudioOutput> audioOutput_;

	// 複数のインスタンスが同時にMXDRVを操作しないようにするための排他
	static QMutex mutex_;

};

#endif // QMDXPLAYER_H
