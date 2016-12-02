#ifndef PLAYLISTMANAGERCLIENTPROXY_H
#define PLAYLISTMANAGERCLIENTPROXY_H

#include <QPointer>
#include "playlistmanager.h"

class PlaylistManagerReplica;

class PlaylistManagerClientProxy : public PlaylistManager
{
public:
	PlaylistManagerClientProxy(QQmlContext* rootContext, QObject *parent = 0);

	// 現在再生中の曲のインデックス
	// 曲がない場合は-1になる。
	Q_INVOKABLE virtual int currentIndex() const;

public slots:
	// プレイリストをクリア
	virtual void clearPlaylist() override;

	// 指定したプレイリストの読み込み、保存
	virtual bool loadPlaylist(const QString& playlistName) override;
	virtual bool savePlaylist(const QString& playlistName) override;

	// デフォルトのプレイリストの読み込み、保存
	virtual bool loadDefaultPlaylist() override;
	virtual bool saveDefaultPlaylist() override;

	// プレイリストにファイルを追加
	virtual bool addFile(const QString& mdxFile) override;
	virtual bool addFolder(const QString& addPath, bool isTopFolder = true) override;

	// インデックスを指定
	virtual void setCurrentIndex(int index) override;

protected slots:
	virtual bool setPlaylist(QByteArray playlist);

protected:
	QSharedPointer<PlaylistManagerReplica> replica_;
};

#endif // PLAYLISTMANAGERCLIENTPROXY_H
