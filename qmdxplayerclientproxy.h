#ifndef QMDXPLAYERCLIENTPROXY_H
#define QMDXPLAYERCLIENTPROXY_H

#include "qmdxplayer.h"
class QMDXPlayerReplica;

class QMDXPlayerClientProxy : public QMDXPlayer
{
public:
	QMDXPlayerClientProxy(PlaylistManager *playlistManager = 0, QObject *parent = 0);

public slots:
	// 曲をロードする。
	// 演奏せずにファイル情報を取得するだけの場合はrenderWavをfalseにする。
	virtual bool loadSong(bool renderWav,
				  const QString& fileName,
				  const QString& pdxPath,
				  unsigned loops,
				  bool enableFadeout) override;
	// 演奏開始
	virtual bool startPlay() override;
	// 演奏中断
	virtual bool stopPlay() override;

	// 曲中のシーク(引数は曲頭からの秒数)
	virtual bool setCurrentPosition(float position) override;

	// 曲送り、曲戻し
	virtual bool stepForward();
	virtual bool stepBackward();

	// インデックスを指定
	virtual bool playFileByIndex(int index);

public:
	// 曲名を取得
	virtual QString title() override;
	// ファイル名を取得
	virtual QString fileName() override;
	// 曲長(秒)を取得
	virtual float duration() override;
	// 曲長(秒)をmm:ssフォーマットで取得
	virtual QString durationString() override;
	// 演奏中の位置(秒)を取得
	virtual float currentPosition() override;
	// 演奏中の位置(秒)をmm:ssフォーマットで取得
	virtual QString currentPositionString() override;
	// 演奏中かどうか取得
	virtual bool isPlaying() override;
	// 曲がロードされているかどうか取得
	virtual bool isSongLoaded() override;

protected:
	QSharedPointer<QMDXPlayerReplica> replica_;

};

#endif // QMDXPLAYERCLIENTPROXY_H
