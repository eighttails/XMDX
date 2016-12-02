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
	connect(replica_.data(), &PlaylistManagerReplica::currentIndexChanged, this, &PlaylistManagerClientProxy::setCurrentIndex);
	connect(replica_.data(), &PlaylistManagerReplica::currentIndexChanged, this, &PlaylistManagerClientProxy::currentIndexChanged);
	connect(replica_.data(), &PlaylistManagerReplica::playlistChanged, this, &PlaylistManagerClientProxy::setPlaylist);
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
	return replica_->loadPlaylist(playlistName).returnValue();
}

bool PlaylistManagerClientProxy::savePlaylist(const QString &playlistName)
{
	return replica_->savePlaylist(playlistName).returnValue();
}

bool PlaylistManagerClientProxy::loadDefaultPlaylist()
{
	return setPlaylist(replica_->getPlaylist().returnValue());
}

bool PlaylistManagerClientProxy::saveDefaultPlaylist()
{
	return replica_->saveDefaultPlaylist().returnValue();
}

bool PlaylistManagerClientProxy::addFile(const QString &mdxFile)
{
	return replica_->addFile(mdxFile).returnValue();
}

bool PlaylistManagerClientProxy::addFolder(const QString &addPath, bool isTopFolder)
{
	return replica_->addFolder(addPath, isTopFolder).returnValue();
}

void PlaylistManagerClientProxy::setCurrentIndex(int index)
{
	return replica_->setCurrentIndex(index);
}

bool PlaylistManagerClientProxy::setPlaylist(QByteArray playlist)
{
	QDataStream stream(playlist);
	return readPlaylist(stream);
}
