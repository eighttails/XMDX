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
    Q_INVOKABLE bool loadPlayList(QString playListName);
    Q_INVOKABLE bool savePlayList(QString playListName);

    // デフォルトのプレイリストの読み込み、保存
    Q_INVOKABLE bool loadDefaultPlaylist();
    Q_INVOKABLE bool saveDefaultPlayList();

    // プレイリストにファイルを追加
    Q_INVOKABLE bool addFile(QString mdxFile);
    Q_INVOKABLE bool addFolder(QString addPath);
signals:

public slots:

private:
    bool loadPlayList(QString path, QString playListName);
    bool savePlayList(QString path, QString playListName);

    void notifyPlayListUpdated();
    QQmlContext* rootContext_;
    QList<QObject*>* playList_;
};

#endif // HELPER_H
