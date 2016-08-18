#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include "qmdxplayer.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);
    QQuickStyle::setStyle("Material");

    QQmlApplicationEngine engine;

    QMDXPlayer mdxPlayer; // 音楽再生用
    QMDXPlayer mdxInfo; // 曲情報取得用
    engine.rootContext()->setContextProperty(QStringLiteral("mdxPlayer"), &mdxPlayer);
    engine.rootContext()->setContextProperty(QStringLiteral("mdxInfo"), &mdxInfo);

#ifdef QT_DEBUG
    engine.rootContext()->setContextProperty("debug", true);
#else
    engine.rootContext()->setContextProperty("debug", false);
#endif

    engine.load(QUrl(QLatin1String("qrc:/qml/main.qml")));
    return app.exec();
}
