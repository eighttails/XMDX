#include "playlistitem.h"

PlayListItem::PlayListItem(QObject *parent) : QObject(parent)
{

}

PlayListItem::PlayListItem(QString title, QString fileName)
	: title_(title), fileName_(fileName)
{

}

QString PlayListItem::title()
{
	return title_;
}

QString PlayListItem::fileName()
{
	return fileName_;
}

void PlayListItem::setTitle(QString title)
{
	title_ = title;
	emit titleChanged();
}

void PlayListItem::setFileName(QString fileName)
{
	fileName_ = fileName;
	emit fileNameChanged();
}
