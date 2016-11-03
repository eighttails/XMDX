#ifndef PLAYLISTITEM_H
#define PLAYLISTITEM_H

#include <QObject>

class PlayListItem : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
	Q_PROPERTY(QString fileName READ fileName WRITE setFileName NOTIFY fileNameChanged)

public:
	explicit PlayListItem(QObject *parent = 0);
	PlayListItem(QString title, QString fileName);

	QString title();
	QString fileName();
signals:
	void titleChanged();
	void fileNameChanged();
public slots:
	void setTitle(QString title);
	void setFileName(QString fileName);
private:
	QString title_;
	QString fileName_;
};

#endif // PLAYLISTITEM_H
