#ifndef PLAYLISTMANAGERSERVICEPROXY_H
#define PLAYLISTMANAGERSERVICEPROXY_H

#include "rep_playlistmanager_source.h"

class PlaylistManager;

class PlaylistManagerServiceProxy : public PlaylistManagerSource
{
public:
	PlaylistManagerServiceProxy(PlaylistManager* playlistManager, QObject *parent = 0);
	virtual int currentIndex() const override;
	virtual void setCurrentIndex(int currentIndex) override;

public slots:
	virtual void clearPlaylist() override;
	virtual bool loadPlaylist(const QString & playlistName) override;
	virtual bool savePlaylist(const QString & playlistName) override;
	virtual bool loadDefaultPlaylist() override;
	virtual bool saveDefaultPlaylist() override;
	virtual bool addFile(const QString & mdxFile) override;
	virtual bool addFolder(const QString & addPath, bool isTopFolder) override;

	virtual QByteArray getPlaylist() override;
protected slots:

protected:
	PlaylistManager* playlistManager_;
};

#endif // PLAYLISTMANAGERSERVICEPROXY_H
