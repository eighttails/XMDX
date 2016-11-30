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
	PlayerService* service = new PlayerService(qApp);
	QMDXPlayer* player = new QMDXPlayer(service);
	QMDXPlayerServiceProxy* proxy = new QMDXPlayerServiceProxy(player, service);

	// プレーヤー側のタイトルが変わったら通知を出す
	QObject::connect(player, &QMDXPlayer::titleChanged, service, &PlayerService::setNotification);

	QRemoteObjectHost* srcNode = new QRemoteObjectHost(QUrl(QStringLiteral("local:replica")), qApp);
	srcNode->enableRemoting(proxy);
}
#endif

QMDXPlayer* createMDXPlayer(PlaylistManager* playlistManager, QObject* parent){
#ifdef Q_OS_ANDROID
	return new QMDXPlayerClientProxy(playlistManager, parent);
#else
	return new QMDXPlayer(playlistManager, parent);
#endif
}

void initGUI()
{
	QApplication::setWindowIcon(QIcon(":/icon/XMDX.png"));
	QQuickStyle::setStyle("Material");

	QQmlApplicationEngine* engine = new QQmlApplicationEngine(qApp);

	PlaylistManager* playlistManager = new PlaylistManager(engine->rootContext(), engine);
	engine->rootContext()->setContextProperty("playlistManager", playlistManager);

	QMDXPlayer* mdxPlayer = createMDXPlayer(playlistManager, engine); // 音楽再生用
	engine->rootContext()->setContextProperty("mdxPlayer", mdxPlayer);

#ifdef QT_DEBUG
	engine->rootContext()->setContextProperty("debug", true);
#else
	engine->rootContext()->setContextProperty("debug", false);
#endif

	// デフォルトのプレイリストをロード
	playlistManager->loadDefaultPlaylist();

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
		// サービスを開始
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
