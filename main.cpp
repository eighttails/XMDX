#include <QApplication>
#include <QIcon>

#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include "qmdxplayer.h"
#include "playlistmanager.h"
#include "playlistitem.h"

#ifdef Q_OS_ANDROID
#include "qmdxplayerclientproxy.h"
#include "playerservice.h"
void initService()
{
	PlayerService* service = new PlayerService(qApp);

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
		initGUI();
	}
#else
	initGUI();
#endif

	return app.exec();
}
