#include <QtDebug>
#include "controlpagehandler.h"

ControlPageHandler::ControlPageHandler(QObject *parent) : ObjectHandler(parent)
{
    displayImageSource = "";
    musicImageSource = "";
    lightImageSource = "";
}

void ControlPageHandler::setDisplayImage(QString src) {
    displayImageSource = getIconPath(src);
}

QString ControlPageHandler::getDisplayImage() {
    return displayImageSource;
}

void ControlPageHandler::setMusicImage(QString src) {
    musicImageSource = getIconPath(src);
}

QString ControlPageHandler::getMusicImage() {
    return musicImageSource;
}

void ControlPageHandler::setLightImage(QString src) {
    lightImageSource = getIconPath(src);
}

QString ControlPageHandler::getLightImage() {
    return lightImageSource;
}

QString ControlPageHandler::getIconPath(QString iconName) {
    QString formedPath = "qrc:/icons/";
    formedPath += iconName;
    formedPath += ".png";
    return formedPath;
}

void ControlPageHandler::displayClicked() {
    qDebug() << "ControlPageHandler::displayClicked()";
    displayLogic.onClicked();
}
void ControlPageHandler::musicClicked() {
    qDebug() << "ControlPageHandler::musicClicked()";
    //musicLogic.onClicked();
}
void ControlPageHandler::lightsClicked(){
    qDebug() << "ControlPageHandler::lightsClicked()";
    //lightsLogic.onClicked();
}
