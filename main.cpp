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
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));

    QMDXPlayer mdxPlayer; // 音楽再生用
    QMDXPlayer mdxInfo; // 曲情報取得用
    engine.rootContext()->setContextProperty(QStringLiteral("mdxPlayer"), &mdxPlayer);
    engine.rootContext()->setContextProperty(QStringLiteral("mdxInfo"), &mdxInfo);

    return app.exec();
}
