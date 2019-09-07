#include <QQmlApplicationEngine>

// Project includes
#include "core.h"

Core::Core(QObject *parent) : QObject(parent)
{

}

void Core::initialize() {
    // Create classes
    controlPage = new ControlPageHandler(this);
    playerPage = new PlayerPageHandler(this);
    // Register classes to QML
    qmlRegisterType<ControlPageHandler>("com.pages.control", 1, 0, "ControlHandler");
    qmlRegisterType<PlayerPageHandler>("com.pages.player", 1, 0, "PlayerHandler");
    // Initialize icon properties
    initIcons();
}

void Core::initIcons() {
    // Initialize control page
    // Display properties
    controlPage->setDisplayImage("projectoroff");
    // Music properties
    controlPage->setAudioImage("musicoff");
    // Light properties
    controlPage->setLightImage("lightsoff");
}
