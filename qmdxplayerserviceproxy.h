#ifndef QMDXPLAYERSERVICEPROXY_H
#define QMDXPLAYERSERVICEPROXY_H
#include "rep_qmdxplayer_source.h"

class QMDXPlayer;

class QMDXPlayerServiceProxy : public QMDXPlayerSource
{
public:
	QMDXPlayerServiceProxy(QMDXPlayer *player, QObject *parent = 0);

	// QMDXPlayerSource interface
public slots:
	virtual bool loadSong(bool renderWav, const QString &fileName, const QString &pdxPath, unsigned loops, bool enableFadeout) override;
	virtual bool startPlay() override;
	virtual bool stopPlay() override;
	virtual bool setCurrentPosition(float position) override;

protected:
	QMDXPlayer* player_;
};

#endif // QMDXPLAYERSERVICEPROXY_H
