// Qt Includes
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QScreen>
#include <QDebug>
// Project includes
#include "core.h"

int main(int argc, char *argv[])
{
    // Load classes
    Core core;
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    // C++, QML integration
    core.initialize();
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    // Update screen size
    QRect rect= app.primaryScreen()->geometry();
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,&app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl) QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);
    // Ensure main screen size matches phone
    engine.rootObjects().at(0)->setProperty("dashboardWidth", rect.width());
    engine.rootObjects().at(0)->setProperty("dashboardHeigh", rect.height());


    return app.exec();
}
