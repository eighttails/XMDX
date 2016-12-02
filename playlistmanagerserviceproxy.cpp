#include "playlistmanagerserviceproxy.h"
#include "playlistmanager.h"

PlaylistManagerServiceProxy::PlaylistManagerServiceProxy(PlaylistManager *playlistManager, QObject *parent)
	: PlaylistManagerSource(parent)
	, playlistManager_(playlistManager)
{
	// 通信用のプロキシと実際のプレーヤーをつなぐ
	connect(playlistManager_, &PlaylistManager::currentIndexChanged, this, &PlaylistManagerServiceProxy::currentIndexChanged);
	connect(playlistManager_, &PlaylistManager::playlistChanged, this, &PlaylistManagerServiceProxy::playlistChanged);
}

int PlaylistManagerServiceProxy::currentIndex() const
{
	return playlistManager_->currentIndex();
}

void PlaylistManagerServiceProxy::setCurrentIndex(int currentIndex)
{
	playlistManager_->setCurrentIndex(currentIndex);
}

void PlaylistManagerServiceProxy::clearPlaylist()
{
	playlistManager_->clearPlaylist();
}

bool PlaylistManagerServiceProxy::loadPlaylist(const QString &playlistName)
{
	return playlistManager_->loadPlaylist(playlistName);
}

bool PlaylistManagerServiceProxy::savePlaylist(const QString &playlistName)
{
	return playlistManager_->savePlaylist(playlistName);
}

bool PlaylistManagerServiceProxy::loadDefaultPlaylist()
{
	return playlistManager_->loadDefaultPlaylist();
}

bool PlaylistManagerServiceProxy::saveDefaultPlaylist()
{
	return playlistManager_->saveDefaultPlaylist();
}

bool PlaylistManagerServiceProxy::addFile(const QString &mdxFile)
{
	return playlistManager_->addFile(mdxFile);
}

bool PlaylistManagerServiceProxy::addFolder(const QString &addPath, bool isTopFolder)
{
	return playlistManager_->addFolder(addPath, isTopFolder);
}

QByteArray PlaylistManagerServiceProxy::getPlaylist()
{
	return playlistManager_->getPlaylist();
}
