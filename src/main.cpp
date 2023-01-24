#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtQml/qqmlregistration.h>

#include <getData.hpp>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    getData osData;

    QQmlApplicationEngine engine;


    QQmlContext* rootContext = engine.rootContext();
    rootContext->setContextProperty("WINDOW_WIDTH", 1000);
    rootContext->setContextProperty("WINDOW_HEIGHT", 500);
    rootContext->setContextProperty("getData", &osData);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
