#ifndef HELPER_H
#define HELPER_H

#include <QObject>
#include <QFileInfo>
#include <QList>
#include <QQmlContext>
#include "playlistitem.h"

class PlaylistManager : public QObject
{
	Q_OBJECT
public:
	explicit PlaylistManager(QQmlContext* rootContext, QObject *parent = 0);

	// ファイル選択、フォルダ選択ダイアログを開く
	// キャンセルされたときは空文字列を返す
	Q_INVOKABLE QString addFileDialog();
	Q_INVOKABLE QString addFolderDialog();
	Q_INVOKABLE QString fileNameFromPath(const QString & filePath) const {
		return QFileInfo(filePath).fileName();
	}

public slots:

	void clearPlaylist();

	// 指定したプレイリストの読み込み、保存
	bool loadPlaylist(const QString& playlistName);
	bool savePlaylist(const QString& playlistName);

	// デフォルトのプレイリストの読み込み、保存
	bool loadDefaultPlaylist();
	bool saveDefaultPlaylist();

	// プレイリストにファイルを追加
	bool addFile(const QString& mdxFile);
	bool addFolder(const QString& addPath, bool isTopFolder = true);

	// ランダム再生時の曲順を取得
	int nextRandom(bool loop);
	int previousRandom(bool loop);

signals:
	// プレイリストが変更されたときにシリアライズされたプレイリストを送る。
	void playlistChanged(QString playlist);

private:
	bool loadPlaylist(QString path, QString playlistName);
	bool savePlaylist(QString path, QString playlistName);

	void makeRandomPlaylist();

	void notifyPlaylistUpdated();
	QQmlContext* rootContext_;
	QList<QObject*> playlist_;

	// ランダム再生用の
	std::vector<int> randomPlaylist_;
	int randomPlayIndex_;
};

#endif // HELPER_H
