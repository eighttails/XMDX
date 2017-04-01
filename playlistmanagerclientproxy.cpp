#include "playlistmanagerclientproxy.h"
#include "rep_playlistmanager_replica.h"

PlaylistManagerClientProxy::PlaylistManagerClientProxy(QQmlContext* rootContext, QObject *parent)
	: PlaylistManager(rootContext, parent)
{
	QRemoteObjectNode* repNode = new QRemoteObjectNode(this);
	repNode->connectToNode(QUrl(QStringLiteral("local:playlistManager")));
	replica_.reset(repNode->acquire<PlaylistManagerReplica>());
	bool res = replica_->waitForSource();
	Q_ASSERT(res);

	// 通信用のレプリカとGUI用のプロキシをつなぐ
	connect(replica_.data(), &PlaylistManagerReplica::currentIndexChanged, this, &PlaylistManagerClientProxy::currentIndexChanged, Qt::QueuedConnection);
	connect(replica_.data(), &PlaylistManagerReplica::playlistChanged, this, &PlaylistManagerClientProxy::setPlaylist, Qt::QueuedConnection);
}

int PlaylistManagerClientProxy::currentIndex() const
{
	return replica_->currentIndex();
}

void PlaylistManagerClientProxy::clearPlaylist()
{
	replica_->clearPlaylist();
}

bool PlaylistManagerClientProxy::loadPlaylist(const QString &playlistName)
{
	auto result = replica_->loadPlaylist(playlistName);
	result.waitForFinished();
	return result.returnValue();
}

bool PlaylistManagerClientProxy::savePlaylist(const QString &playlistName)
{
	auto result = replica_->savePlaylist(playlistName);
	result.waitForFinished();
	return result.returnValue();
}

bool PlaylistManagerClientProxy::loadDefaultPlaylist()
{
	auto result = replica_->getPlaylist();
	result.waitForFinished();
	return setPlaylist(result.returnValue());
}

bool PlaylistManagerClientProxy::saveDefaultPlaylist()
{
	auto result = replica_->saveDefaultPlaylist();
	result.waitForFinished();
	return result.returnValue();
}

bool PlaylistManagerClientProxy::addFile(const QString &mdxFile)
{
	auto result = replica_->addFile(mdxFile);
	result.waitForFinished();
	return result.returnValue();
}

bool PlaylistManagerClientProxy::addFolder(const QString &addPath, bool isTopFolder)
{
	auto result = replica_->addFolder(addPath, isTopFolder);
	result.waitForFinished();
	return result.returnValue();
}

void PlaylistManagerClientProxy::setCurrentIndex(int index)
{
	return replica_->pushCurrentIndex(index);
}

bool PlaylistManagerClientProxy::setPlaylist(QByteArray playlist)
{
	QDataStream stream(playlist);
	return readPlaylist(stream);
}
