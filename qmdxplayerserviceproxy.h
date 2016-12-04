#ifndef QMDXPLAYERSERVICEPROXY_H
#define QMDXPLAYERSERVICEPROXY_H
#include <QPointer>
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
	virtual bool stepForward() override;
	virtual bool stepBackward() override;
	virtual bool playFileByIndex(int index) override;

	virtual QString title() override;
	virtual QString fileName() override;
	virtual float duration() override;
	virtual QString durationString() override;
	virtual float currentPosition() override;
	virtual QString currentPositionString() override;
	virtual bool isPlaying() override;
	virtual bool isSongLoaded() override;

protected:
	QPointer<QMDXPlayer> player_;

	// QMDXPlayerSource interface

};

#endif // QMDXPLAYERSERVICEPROXY_H
