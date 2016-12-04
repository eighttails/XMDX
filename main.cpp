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
#include "playlistmanagerclientproxy.h"
#include "playlistmanagerserviceproxy.h"

void initService()
{
	// Androidようのサービスを生成
	PlayerService* service = new PlayerService(qApp);

	// プレイリストを生成
	PlaylistManager* playlistManager = new PlaylistManager(nullptr, service);
	PlaylistManagerServiceProxy* playlistManagerProxy = new PlaylistManagerServiceProxy(playlistManager, service);
	// デフォルトのプレイリストをロード
	playlistManager->loadDefaultPlaylist();

	// プレーヤー本体を生成
	QMDXPlayer* player = new QMDXPlayer(playlistManager, service);
	QMDXPlayerServiceProxy* playerProxy = new QMDXPlayerServiceProxy(player, service);
	service->setMdxPlayer(player);
	// 曲の再生が終了したら次の曲を再生
	QObject::connect(player, &QMDXPlayer::songPlayFinished, player, &QMDXPlayer::stepForward);

	// プレーヤー側のタイトルが変わったら通知を出す
	QObject::connect(player, &QMDXPlayer::titleChanged, service, &PlayerService::setNotification);
	QObject::connect(playlistManager, &PlaylistManager::songAddedToPlaylist, service, &PlayerService::setNotification);

	// リモート通信を有効化
	QRemoteObjectHost* playerNode = new QRemoteObjectHost(QUrl(QStringLiteral("local:player")), qApp);
	playerNode->enableRemoting(playerProxy);
	QRemoteObjectHost* playlistManagerNode = new QRemoteObjectHost(QUrl(QStringLiteral("local:playlistManager")), qApp);
	playlistManagerNode->enableRemoting(playlistManagerProxy);
}
#endif

QMDXPlayer* createMDXPlayer(PlaylistManager* playlistManager, QObject* parent){
#ifdef Q_OS_ANDROID
	return new QMDXPlayerClientProxy(playlistManager, parent);
	// 曲の再生が終了したら次の曲を再生する処理はサービス側で行うため、ここでは
	// 他プラットフォームで行っている下記のconnectは実施しない。
#else
	QMDXPlayer* player = new QMDXPlayer(playlistManager, parent);
	// 曲の再生が終了したら次の曲を再生
	QObject::connect(player, &QMDXPlayer::songPlayFinished, player, &QMDXPlayer::stepForward);
	return player;
#endif
}

PlaylistManager* createPlaylistManager(QQmlContext* context, QObject* parent)
{
#ifdef Q_OS_ANDROID
	return new PlaylistManagerClientProxy(context, parent);
#else
	return new PlaylistManager(context, parent);
#endif

}

void initGUI()
{
	QApplication::setWindowIcon(QIcon(":/icon/XMDX.png"));
	QQuickStyle::setStyle("Material");

	QQmlApplicationEngine* engine = new QQmlApplicationEngine(qApp);

	PlaylistManager* playlistManager = createPlaylistManager(engine->rootContext(), engine);
	engine->rootContext()->setContextProperty("playlistManager", playlistManager);

	QMDXPlayer* mdxPlayer = createMDXPlayer(playlistManager, engine); // 音楽再生用
	engine->rootContext()->setContextProperty("mdxPlayer", mdxPlayer);

#ifdef QT_DEBUG
	engine->rootContext()->setContextProperty("debug", QVariant::fromValue(true));
#else
	engine->rootContext()->setContextProperty("debug", QVariant::fromValue(false));
#endif

	// デフォルトのプレイリストをロード
	playlistManager->loadDefaultPlaylist();

	engine->load(QUrl(QLatin1String("qrc:/qml/main.qml")));
}


int main(int argc, char *argv[])
{
	QApplication::setApplicationName("XMDX");
	QApplication::setOrganizationDomain("eighttails.org");
	QApplication::setOrganizationName("eighttails");

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
