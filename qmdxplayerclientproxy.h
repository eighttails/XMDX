#ifndef QMDXPLAYERCLIENTPROXY_H
#define QMDXPLAYERCLIENTPROXY_H

#include "qmdxplayer.h"
#include "rep_qmdxplayer_replica.h"

class QMDXPlayerClientProxy : public QMDXPlayer
{
public:
	QMDXPlayerClientProxy(QObject *parent = 0);

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

public:
	virtual float currentPosition() override;
	// 演奏中の位置(秒)をmm:ssフォーマットで取得
	virtual QString currentPositionString() override;
	// 演奏中かどうか取得
	virtual bool isPlaying() override;

protected slots:
	// 曲名を設定
	virtual void setTitle(QString title);
	// ファイル名を設定
	virtual void setFileName(QString fileName);
	// 曲長(秒)を設定
	virtual void setDuration(float duration);
	// 演奏中の位置(秒)を設定(内部用)
	virtual void setCurrentPositionInternal(float currentPosition);
	// 演奏中の位置(秒)をmm:ssフォーマットで設定
	virtual void setCurrentPositionString(QString currentPositionString);
	// 演奏中かどうか設定
	virtual void setIsPlaying(bool isPlaying);
	// 曲がロードされているかどうか設定
	virtual void setIsSongLoaded(bool isSongLoaded);

protected:
	QMDXPlayerReplica replica_;

	float currentPosition_;
	QString currentPositionString_;
	bool isPlaying_;
};

#endif // QMDXPLAYERCLIENTPROXY_H
