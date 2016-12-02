#ifndef PLAYLISTITEM_H
#define PLAYLISTITEM_H

#include <QObject>

class PlaylistItem : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
	Q_PROPERTY(QString fileName READ fileName WRITE setFileName NOTIFY fileNameChanged)

public:
	explicit PlaylistItem(QObject *parent = 0);
	PlaylistItem(QString title, QString fileName);

	QString title() const;
	QString fileName() const;
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
