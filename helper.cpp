#include <QStandardPaths>
#include <QDir>
#include <QDataStream>
#include "helper.h"
#include "playlistitem.h"

Helper::Helper(QQmlContext* rootContext, QList<QObject*>* playList, QObject *parent)
    : QObject(parent)
    , rootContext_(rootContext)
    , playList_(playList)
{
    Q_ASSERT(rootContext);
    Q_ASSERT(playList);
}

void Helper::clearPlayList()
{
    while(!playList_->isEmpty()){
        delete playList_->takeFirst();
    }
    notifyPlayListUpdated();
}

bool Helper::loadPlayList(QString playListName)
{
    QString loadPath = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    QString playListFileName = loadPath + QDir::separator() + playListName;

    QFile playListFile(playListFileName);
    QDataStream stream(&playListFile);
    if(!playListFile.open(QIODevice::ReadOnly)){
        return false;
    }
    clearPlayList();
    while(!stream.atEnd()){
        QString title, fileName;
        stream >> title >> fileName;
        playList_->append(new PlayListItem(title, fileName));
    }
    notifyPlayListUpdated();
}

bool Helper::savePlayList(QString playListName)
{
    QString savePath = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    QString playListFileName = savePath + QDir::separator() + playListName;

    QDir d(savePath);
    if(!d.exists()){
       if (!d.mkpath(savePath))
           return false;
    }
    QFile playListFile(playListFileName);
    QDataStream stream(&playListFile);
    if(!playListFile.open(QIODevice::WriteOnly)){
        return false;
    }
    foreach(auto item, *playList_){
        PlayListItem* playListItem = qobject_cast<PlayListItem*>(item);
        stream << playListItem->title() << playListItem->fileName();
    }
}

void Helper::notifyPlayListUpdated()
{
    rootContext_->setContextProperty("playList", QVariant::fromValue(*playList_));
}
