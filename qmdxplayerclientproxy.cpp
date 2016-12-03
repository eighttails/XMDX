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
	connect(replica_.data(), &QMDXPlayerReplica::isPlayingChanged, this, &QMDXPlayerClientProxy::setIsPlaying, Qt::QueuedConnection);
	connect(replica_.data(), &QMDXPlayerReplica::isSongLoadedChanged, this, &QMDXPlayerClientProxy::setIsSongLoaded, Qt::QueuedConnection);
	connect(replica_.data(), &QMDXPlayerReplica::titleChanged, this, &QMDXPlayerClientProxy::setTitle, Qt::QueuedConnection);
	connect(replica_.data(), &QMDXPlayerReplica::fileNameChanged, this, &QMDXPlayerClientProxy::setFileName, Qt::QueuedConnection);
	connect(replica_.data(), &QMDXPlayerReplica::durationChanged, this, &QMDXPlayerClientProxy::setDuration, Qt::QueuedConnection);
	connect(replica_.data(), &QMDXPlayerReplica::durationStringChanged, this, &QMDXPlayerClientProxy::durationStringChanged, Qt::QueuedConnection);
	connect(replica_.data(), &QMDXPlayerReplica::currentPositionChanged, this, &QMDXPlayerClientProxy::setCurrentPositionInternal, Qt::QueuedConnection);
	connect(replica_.data(), &QMDXPlayerReplica::currentPositionStringChanged, this, &QMDXPlayerClientProxy::setCurrentPositionString, Qt::QueuedConnection);
	connect(replica_.data(), &QMDXPlayerReplica::songPlayFinished, this, &QMDXPlayerClientProxy::songPlayFinished, Qt::QueuedConnection);
}

bool QMDXPlayerClientProxy::loadSong(bool renderWav, const QString &fileName, const QString &pdxPath, unsigned loops, bool enableFadeout)
{
	// レンダリングを行わない場合は曲名取得のため、普通に読み込む。
	if(!renderWav){
		return QMDXPlayer::loadSong(renderWav, fileName, pdxPath, loops, enableFadeout);
	} else {
		//レンダリングを行う場合はサービスに依頼する
		auto result = replica_->loadSong(renderWav, fileName, pdxPath, loops, enableFadeout);
		result.waitForFinished();
		return result.returnValue();
	}
}

bool QMDXPlayerClientProxy::startPlay()
{
	auto result = replica_->startPlay();
	result.waitForFinished();
	return result.returnValue();
}

bool QMDXPlayerClientProxy::stopPlay()
{
	auto result = replica_->stopPlay();
	result.waitForFinished();
	return result.returnValue();
}

bool QMDXPlayerClientProxy::setCurrentPosition(float position)
{
	auto result = replica_->setCurrentPosition(position);
	result.waitForFinished();
	return result.returnValue();
}

bool QMDXPlayerClientProxy::stepForward()
{
	auto result = replica_->stepForward();
	result.waitForFinished();
	return result.returnValue();
}

bool QMDXPlayerClientProxy::stepBackward()
{
	auto result = replica_->stepBackward();
	result.waitForFinished();
	return result.returnValue();
}

bool QMDXPlayerClientProxy::playFileByIndex(int index)
{
	auto result = replica_->playFileByIndex(index);
	result.waitForFinished();
	return result.returnValue();
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

