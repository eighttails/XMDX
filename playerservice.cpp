#include <QtAndroid>
#include <QTimer>
#include "playerservice.h"
#include "qmdxplayer.h"

PlayerService::PlayerService(QObject *parent)
	: QObject(parent)
	, idleCount_(0)
{
	connect(this, &PlayerService::notificationChanged, this, &PlayerService::updateAndroidNotification);

	// アイドル状態をチェックするためのタイマー
	// 10分間停止状態が続いたらサービスを終了する。
	QTimer* timer = new QTimer(this);
	timer->setInterval(1000 * 60);
	connect(timer, &QTimer::timeout, this, &PlayerService::onIdle);
	timer->start();
}

void PlayerService::setMdxPlayer(QMDXPlayer *mdxPlayer)
{
	mdxPlayer_ = mdxPlayer;
}

void PlayerService::setNotification(const QString &notification)
{
	if (m_notification == notification)
		return;

	m_notification = notification;
	emit notificationChanged();
}

QString PlayerService::notification() const
{
	return m_notification;
}

void PlayerService::updateAndroidNotification()
{
	QAndroidJniObject javaNotification = QAndroidJniObject::fromString(m_notification);
	QAndroidJniObject::callStaticMethod<void>("org/eighttails/xmdx/PlayerService",
											  "notify",
											  "(Ljava/lang/String;)V",
											  javaNotification.object<jstring>());
}

void PlayerService::onIdle()
{
	if(mdxPlayer_->isPlaying()){
		idleCount_ = 0;
	} else {
		idleCount_++;
	}
	// サービスを終了
	if(idleCount_ >= 10){
		QAndroidJniObject::callStaticMethod<void>("org/eighttails/xmdx/PlayerService",
												  "stopPlayerService");
	}
}
