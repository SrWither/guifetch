#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtQml/qqmlregistration.h>

#include <getData.hpp>
#include <getImage.hpp>

int main(int argc, char *argv[]) {
  // Init Qt6
  QGuiApplication app(argc, argv);

  // Init data classes
  getData osData;
  getImage osImageData;

  // Init qml
  QQmlApplicationEngine engine;

  // Data to qml
  QQmlContext *rootContext = engine.rootContext();
  rootContext->setContextProperty("WINDOW_WIDTH", 1100);
  rootContext->setContextProperty("WINDOW_HEIGHT", 550);
  rootContext->setContextProperty("getData", &osData);
  rootContext->setContextProperty("getImage", &osImageData);

  // Load qml file
  engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

  // start app
  return app.exec();
}
