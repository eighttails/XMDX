#include <QApplication>
#include <QIcon>

#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include "qmdxplayer.h"
#include "helper.h"
#include "playlistitem.h"

#ifdef Q_OS_ANDROID
#include "qmdxplayerclientproxy.h"
#include "playerservice.h"
void initService()
{
PlayerService service;

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

	QObjectList* playList = new QObjectList;
	engine->rootContext()->setContextProperty("playList", QVariant::fromValue(*playList));

	Helper* helper = new Helper(engine->rootContext(), playList, engine);
	engine->rootContext()->setContextProperty("appHelper", helper);

#ifdef QT_DEBUG
	engine->rootContext()->setContextProperty("debug", true);
#else
	engine->rootContext()->setContextProperty("debug", false);
#endif

	// デフォルトのプレイリストをロード
	helper->loadDefaultPlaylist();

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
