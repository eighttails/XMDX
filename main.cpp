#include <QApplication>
#include <QIcon>

#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include "qmdxplayer.h"
#include "playlistmanager.h"
#include "playlistitem.h"

#ifdef Q_OS_ANDROID
#include <QtAndroid>
#include "playerservice.h"
#include "qmdxplayerclientproxy.h"
#include "qmdxplayerserviceproxy.h"

void initService()
{
	QMDXPlayer* player = new QMDXPlayer(qApp);
	PlayerService* service = new PlayerService(player);
	QMDXPlayerServiceProxy* proxy = new QMDXPlayerServiceProxy(player, player);

	// プレーヤー側のタイトルが変わったら通知を出す
	QObject::connect(player, &QMDXPlayer::titleChanged, service, &PlayerService::setNotification);

	QRemoteObjectHost* srcNode = new QRemoteObjectHost(QUrl(QStringLiteral("local:replica")), qApp);
	srcNode->enableRemoting(proxy);
}
#endif

QMDXPlayer* createMDXPlayer(QQmlApplicationEngine* engine){
#ifdef Q_OS_ANDROID
	return new QMDXPlayerClientProxy(engine);
#else
	return new QMDXPlayer(engine);
#endif
}

void initGUI()
{
	QApplication::setWindowIcon(QIcon(":/icon/XMDX.png"));
	QQuickStyle::setStyle("Material");

	QQmlApplicationEngine* engine = new QQmlApplicationEngine(qApp);

	QMDXPlayer* mdxPlayer = createMDXPlayer(engine); // 音楽再生用
	engine->rootContext()->setContextProperty("mdxPlayer", mdxPlayer);

	PlaylistManager* playListManager = new PlaylistManager(engine->rootContext(), engine);
	engine->rootContext()->setContextProperty("playlistManager", playListManager);

#ifdef QT_DEBUG
	engine->rootContext()->setContextProperty("debug", true);
#else
	engine->rootContext()->setContextProperty("debug", false);
#endif

	// デフォルトのプレイリストをロード
	playListManager->loadDefaultPlaylist();

	engine->load(QUrl(QLatin1String("qrc:/qml/main.qml")));
}


int main(int argc, char *argv[])
{
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QApplication app(argc, argv);

#ifdef Q_OS_ANDROID
	if(argc >= 2 && QString(argv[1]) == "-service"){
		initService();
	} else {
		QAndroidJniObject::callStaticMethod<void>("org/eighttails/xmdx/PlayerService",
												  "startPlayerService",
												  "(Landroid/content/Context;)V",
												  QtAndroid::androidActivity().object());

		initGUI();
	}
#else
	initGUI();
#endif

	return app.exec();
}
