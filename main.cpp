// Qt Includes
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QScreen>
#include <QDebug>
// Project includes
#include "controlpagehandler.h"
#include "playerpagehandler.h"

int main(int argc, char *argv[])
{
    // Load classes
    ControlPageHandler controlPage;
    PlayerPageHandler playerPage;
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
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
    // C++, QML integration
    qmlRegisterType<ControlPageHandler>("com.pages.control", 1, 0, "ControlHandler");
    qmlRegisterType<PlayerPageHandler>("com.pages.player", 1, 0, "PlayerHandler");
    return app.exec();
}
