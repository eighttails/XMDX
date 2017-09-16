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
	Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged)
public:
	explicit PlaylistManager(QQmlContext* rootContext, QObject *parent = 0);

	// ファイル選択、フォルダ選択ダイアログを開く
	// キャンセルされたときは空文字列を返す
	Q_INVOKABLE QString addFileDialog();
	Q_INVOKABLE QString addFolderDialog();
	Q_INVOKABLE QString fileNameFromPath(const QString & filePath) const {
		return QFileInfo(filePath).fileName();
	}

	// 次、または前の曲のファイル名を取得
	Q_INVOKABLE QString next(bool random, bool loop);
	Q_INVOKABLE QString previous(bool random, bool loop);

	// 現在再生中の曲のインデックス
	// 曲がない場合は-1になる。
	Q_INVOKABLE virtual int currentIndex() const;

	Q_INVOKABLE const PlaylistItem *currentPlaylistItem();
public slots:
	// プレイリストをクリア
	virtual void clearPlaylist();

	// 指定したプレイリストの読み込み、保存
	virtual bool loadPlaylist(const QString& playlistName);
	virtual bool savePlaylist(const QString& playlistName);

	// デフォルトのプレイリストの読み込み、保存
	virtual bool loadDefaultPlaylist();
	virtual bool saveDefaultPlaylist();

	// プレイリストにファイルを追加
	virtual bool addFile(const QString& mdxFile);
	virtual bool addFolder(const QString& addPath, bool isTopFolder = true);

	// インデックスを指定
	virtual void setCurrentIndex(int index);

	// シリアライズしたプレイリストを取得
	virtual QByteArray getPlaylist();

signals:
	// プレイリストが変更されたときにシリアライズされたプレイリストを送る。
	void playlistChanged(QByteArray playlist);

	// 再生中の曲のインデックスが変更された場合に通知を送る
	void currentIndexChanged(int index);

	// プレイリストに曲が追加された場合にタイトルを通知
	void songAddedToPlaylist(QString title);

protected:
	bool loadPlaylist(QString path, QString playlistName);
	bool savePlaylist(QString path, QString playlistName);

	bool readPlaylist(QDataStream& stream);
	bool writePlaylist(QDataStream& stream);

	void makeRandomPlaylist();

	void notifyPlaylistUpdated();
	QQmlContext* rootContext_;
	QList<QObject*> playlist_;

	// Android用のストレージアクセス許可要求
	bool requestStoragePermission();

	// 現在再生中の曲を示すインデックス。曲がない場合は-1を示す
	int currentIndex_;

	// ランダム再生用の
	std::vector<int> randomPlaylist_;
	int randomPlayIndex_;
	void resetCurrentIndex();
};

#endif // HELPER_H
