#include "qmdxplayerclientproxy.h"
#include "rep_qmdxplayer_replica.h"

QMDXPlayerClientProxy::QMDXPlayerClientProxy(PlaylistManager *playlistManager, QObject* parent)
	: QMDXPlayer(playlistManager, parent)
	, currentPosition_(0)
{
	QRemoteObjectNode* repNode = new QRemoteObjectNode(this);
	repNode->connectToNode(QUrl(QStringLiteral("local:player")));
	replica_.reset(repNode->acquire<QMDXPlayerReplica>());
	bool res = replica_->waitForSource();
	Q_ASSERT(res);

	// 通信用のレプリカとGUI用のプロキシをつなぐ
	connect(replica_.data(), &QMDXPlayerReplica::isPlayingChanged, this, &QMDXPlayerClientProxy::setIsPlaying);
	connect(replica_.data(), &QMDXPlayerReplica::isSongLoadedChanged, this, &QMDXPlayerClientProxy::setIsSongLoaded);
	connect(replica_.data(), &QMDXPlayerReplica::titleChanged, this, &QMDXPlayerClientProxy::setTitle);
	connect(replica_.data(), &QMDXPlayerReplica::fileNameChanged, this, &QMDXPlayerClientProxy::setFileName);
	connect(replica_.data(), &QMDXPlayerReplica::durationChanged, this, &QMDXPlayerClientProxy::setDuration);
	connect(replica_.data(), &QMDXPlayerReplica::durationStringChanged, this, &QMDXPlayerClientProxy::durationStringChanged);
	connect(replica_.data(), &QMDXPlayerReplica::currentPositionChanged, this, &QMDXPlayerClientProxy::setCurrentPositionInternal);
	connect(replica_.data(), &QMDXPlayerReplica::currentPositionStringChanged, this, &QMDXPlayerClientProxy::setCurrentPositionString);
	connect(replica_.data(), &QMDXPlayerReplica::songPlayFinished, this, &QMDXPlayerClientProxy::songPlayFinished);
}

bool QMDXPlayerClientProxy::loadSong(bool renderWav, const QString &fileName, const QString &pdxPath, unsigned loops, bool enableFadeout)
{
	// レンダリングを行わない場合は曲名取得のため、普通に読み込む。
	if(!renderWav){
		return QMDXPlayer::loadSong(renderWav, fileName, pdxPath, loops, enableFadeout);
	} else {
		//レンダリングを行う場合はサービスに依頼する
		return replica_->loadSong(renderWav, fileName, pdxPath, loops, enableFadeout).returnValue();
	}
}

bool QMDXPlayerClientProxy::startPlay()
{
	return replica_->startPlay().returnValue();
}

bool QMDXPlayerClientProxy::stopPlay()
{
	return replica_->stopPlay().returnValue();
}

bool QMDXPlayerClientProxy::setCurrentPosition(float position)
{
	return replica_->setCurrentPosition(position).returnValue();
}

bool QMDXPlayerClientProxy::stepForward()
{
	return replica_->stepForward().returnValue();
}

bool QMDXPlayerClientProxy::stepBackward()
{
	return replica_->stepBackward().returnValue();
}

bool QMDXPlayerClientProxy::playFileByIndex(int index)
{
	return replica_->playFileByIndex(index).returnValue();
}

float QMDXPlayerClientProxy::currentPosition()
{
	return currentPosition_;
}

QString QMDXPlayerClientProxy::currentPositionString()
{
	return currentPositionString_;
}

bool QMDXPlayerClientProxy::isPlaying()
{
	return isPlaying_;
}

void QMDXPlayerClientProxy::setTitle(QString title)
{
	title_ = title;
	emit titleChanged(title);
}

void QMDXPlayerClientProxy::setFileName(QString fileName)
{
	fileName_ = fileName;
	emit fileNameChanged(fileName);
}

void QMDXPlayerClientProxy::setDuration(float duration)
{
	duration_ = duration;
	emit durationChanged(duration);
	emit durationStringChanged(durationString());
}

void QMDXPlayerClientProxy::setCurrentPositionInternal(float currentPosition)
{
	currentPosition_ = currentPosition;
	emit currentPositionChanged(currentPosition);
}

void QMDXPlayerClientProxy::setCurrentPositionString(QString currentPositionString)
{
	currentPositionString_ = currentPositionString;
	emit currentPositionStringChanged(currentPositionString);
}

void QMDXPlayerClientProxy::setIsPlaying(bool isPlaying)
{
	isPlaying_ = isPlaying;
	emit isPlayingChanged(isPlaying);
}

void QMDXPlayerClientProxy::setIsSongLoaded(bool isSongLoaded)
{
	isSongLoaded_ = isSongLoaded;
	emit isSongLoadedChanged(isSongLoaded);
}

