#include <QQmlApplicationEngine>

// Project includes
#include "core.h"

Core::Core(QObject *parent) : QObject(parent)
{

}

void Core::initialize() {
    controlPage = new ControlPageHandler(this);
    playerPage = new PlayerPageHandler(this);
    qmlRegisterType<ControlPageHandler>("com.pages.control", 1, 0, "ControlHandler");
    qmlRegisterType<PlayerPageHandler>("com.pages.player", 1, 0, "PlayerHandler");
    initIcons();    // Init properties for icons
}

void Core::initIcons() {
    controlPage->setDisplayImage("projectoroff");
    controlPage->setMusicImage("musicoff");
    controlPage->setLightImage("lightsoff");
}
