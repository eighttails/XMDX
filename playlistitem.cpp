#include "playlistitem.h"

PlaylistItem::PlaylistItem(QObject *parent) : QObject(parent)
{

}

PlaylistItem::PlaylistItem(QString title, QString fileName)
	: title_(title), fileName_(fileName)
{

}

QString PlaylistItem::title()
{
	return title_;
}

QString PlaylistItem::fileName()
{
	return fileName_;
}

void PlaylistItem::setTitle(QString title)
{
	title_ = title;
	emit titleChanged();
}

void PlaylistItem::setFileName(QString fileName)
{
	fileName_ = fileName;
	emit fileNameChanged();
}
