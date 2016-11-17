#include "qmdxplayerserviceproxy.h"
#include "qmdxplayer.h"

QMDXPlayerServiceProxy::QMDXPlayerServiceProxy(QMDXPlayer* player, QObject *parent)
	: QMDXPlayerSource(parent)
	, player_(player)
{
	// 通信用のプロキシと実際のプレーヤーをつなぐ
	connect(player_, &QMDXPlayer::isPlayingChanged, this, &QMDXPlayerServiceProxy::isPlayingChanged);
	connect(player_, &QMDXPlayer::isSongLoadedChanged, this, &QMDXPlayerServiceProxy::isSongLoadedChanged);
	connect(player_, &QMDXPlayer::titleChanged, this, &QMDXPlayerServiceProxy::titleChanged);
	connect(player_, &QMDXPlayer::fileNameChanged, this, &QMDXPlayerServiceProxy::fileNameChanged);
	connect(player_, &QMDXPlayer::durationChanged, this, &QMDXPlayerServiceProxy::durationChanged);
	connect(player_, &QMDXPlayer::durationStringChanged, this, &QMDXPlayerServiceProxy::durationStringChanged);
	connect(player_, &QMDXPlayer::currentPositionChanged, this, &QMDXPlayerServiceProxy::currentPositionChanged);
	connect(player_, &QMDXPlayer::currentPositionStringChanged, this, &QMDXPlayerServiceProxy::currentPositionStringChanged);
	connect(player_, &QMDXPlayer::songPlayFinished, this, &QMDXPlayerServiceProxy::songPlayFinished);
}


bool QMDXPlayerServiceProxy::loadSong(bool renderWav, const QString &fileName, const QString &pdxPath, unsigned loops, bool enableFadeout)
{
	return player_->loadSong(renderWav, fileName, pdxPath, loops, enableFadeout);
}

bool QMDXPlayerServiceProxy::startPlay()
{
	return player_->startPlay();
}

bool QMDXPlayerServiceProxy::stopPlay()
{
	return player_->stopPlay();
}

bool QMDXPlayerServiceProxy::setCurrentPosition(float position)
{
	return player_->setCurrentPosition(position);
}
