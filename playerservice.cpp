#include <QtAndroidExtras/QAndroidJniObject>
#include "playerservice.h"

PlayerService::PlayerService(QObject *parent)
	: QObject(parent)
{
	connect(this, SIGNAL(notificationChanged()), this, SLOT(updateAndroidNotification()));
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
