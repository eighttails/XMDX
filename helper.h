#ifndef HELPER_H
#define HELPER_H

#include <QObject>
#include <QFileInfo>
#include <QList>
#include <QQmlContext>
#include "playlistitem.h"

class Helper : public QObject
{
	Q_OBJECT
public:
	explicit Helper(QQmlContext* rootContext, QList<QObject*>* playList, QObject *parent = 0);
	Q_INVOKABLE QString fileNameFromPath(const QString & filePath) const {
		return QFileInfo(filePath).fileName();
	}
	Q_INVOKABLE void clearPlayList();

	// 指定したプレイリストの読み込み、保存
	Q_INVOKABLE bool loadPlayList(const QString& playListName);
	Q_INVOKABLE bool savePlayList(const QString& playListName);

	// デフォルトのプレイリストの読み込み、保存
	Q_INVOKABLE bool loadDefaultPlaylist();
	Q_INVOKABLE bool saveDefaultPlayList();

	// プレイリストにファイルを追加
	Q_INVOKABLE bool addFile(const QString& mdxFile);
	Q_INVOKABLE bool addFolder(const QString& addPath, bool isTopFolder = true);

	// ファイル選択、フォルダ選択ダイアログを開く
	// キャンセルされたときは空文字列を返す
	Q_INVOKABLE QString addFileDialog();
	Q_INVOKABLE QString addFolderDialog();

	// ランダム再生時の曲順を取得
	Q_INVOKABLE int nextRandom(bool loop);
	Q_INVOKABLE int previousRandom(bool loop);

signals:

public slots:

private:
	bool loadPlayList(QString path, QString playListName);
	bool savePlayList(QString path, QString playListName);

	void makeRandomPlayList();

	void notifyPlayListUpdated();
	QQmlContext* rootContext_;
	QList<QObject*>* playList_;

	// ランダム再生用の
	std::vector<int> randomPlayList_;
	int randomPlayIndex_;
};

#endif // HELPER_H
