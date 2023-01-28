#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtQml/qqmlregistration.h>

#include <getData.hpp>
#include <getImage.hpp>

int main(int argc, char *argv[]) {
  QGuiApplication app(argc, argv);

  getData osData;
  getImage osImageData;

  QQmlApplicationEngine engine;

  QQmlContext *rootContext = engine.rootContext();
  rootContext->setContextProperty("WINDOW_WIDTH", 1100);
  rootContext->setContextProperty("WINDOW_HEIGHT", 550);
  rootContext->setContextProperty("getData", &osData);
  rootContext->setContextProperty("getImage", &osImageData);

  engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

  return app.exec();
}
