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

    Q_INVOKABLE bool loadPlayList(QString playListName);
    Q_INVOKABLE bool savePlayList(QString playListName);

signals:

public slots:

private:
    void notifyPlayListUpdated();
    QQmlContext* rootContext_;
    QList<QObject*>* playList_;
};

#endif // HELPER_H
