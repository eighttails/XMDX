#include "qmdxplayerclientproxy.h"
#include "rep_qmdxplayer_replica.h"

QMDXPlayerClientProxy::QMDXPlayerClientProxy(PlaylistManager *playlistManager, QObject* parent)
	: QMDXPlayer(playlistManager, parent)
{
	QRemoteObjectNode* repNode = new QRemoteObjectNode(this);
	repNode->connectToNode(QUrl(QStringLiteral("local:player")));
	replica_.reset(repNode->acquire<QMDXPlayerReplica>());
	bool res = replica_->waitForSource();
	Q_ASSERT(res);

	// 通信用のレプリカとGUI用のプロキシをつなぐ
	connect(replica_.data(), &QMDXPlayerReplica::isPlayingChanged, this, &QMDXPlayerClientProxy::isPlayingChanged, Qt::QueuedConnection);
	connect(replica_.data(), &QMDXPlayerReplica::isSongLoadedChanged, this, &QMDXPlayerClientProxy::isSongLoadedChanged, Qt::QueuedConnection);
	connect(replica_.data(), &QMDXPlayerReplica::titleChanged, this, &QMDXPlayerClientProxy::titleChanged, Qt::QueuedConnection);
	connect(replica_.data(), &QMDXPlayerReplica::fileNameChanged, this, &QMDXPlayerClientProxy::fileNameChanged, Qt::QueuedConnection);
	connect(replica_.data(), &QMDXPlayerReplica::durationChanged, this, &QMDXPlayerClientProxy::durationChanged, Qt::QueuedConnection);
	connect(replica_.data(), &QMDXPlayerReplica::durationStringChanged, this, &QMDXPlayerClientProxy::durationStringChanged, Qt::QueuedConnection);
	connect(replica_.data(), &QMDXPlayerReplica::currentPositionChanged, this, &QMDXPlayerClientProxy::currentPositionChanged, Qt::QueuedConnection);
	connect(replica_.data(), &QMDXPlayerReplica::currentPositionStringChanged, this, &QMDXPlayerClientProxy::currentPositionStringChanged, Qt::QueuedConnection);
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

QString QMDXPlayerClientProxy::title()
{
	auto result = replica_->title();
	result.waitForFinished();
	return result.returnValue();
}

QString QMDXPlayerClientProxy::fileName()
{
	auto result = replica_->fileName();
	result.waitForFinished();
	return result.returnValue();
}

float QMDXPlayerClientProxy::duration()
{
	auto result = replica_->duration();
	result.waitForFinished();
	return result.returnValue();
}

QString QMDXPlayerClientProxy::durationString()
{
	auto result = replica_->durationString();
	result.waitForFinished();
	return result.returnValue();
}

float QMDXPlayerClientProxy::currentPosition()
{
	auto result = replica_->currentPosition();
	result.waitForFinished();
	return result.returnValue();}

QString QMDXPlayerClientProxy::currentPositionString()
{
	auto result = replica_->currentPositionString();
	result.waitForFinished();
	return result.returnValue();}

bool QMDXPlayerClientProxy::isPlaying()
{
	auto result = replica_->isPlaying();
	result.waitForFinished();
	return result.returnValue();
}

bool QMDXPlayerClientProxy::isSongLoaded()
{
	auto result = replica_->isSongLoaded();
	result.waitForFinished();
	return result.returnValue();
}

