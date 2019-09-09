#include <QtDebug>
#include "controlpagehandler.h"

ControlPageHandler::ControlPageHandler(QObject *parent) : ObjectHandler(parent)
{
    displayImageSource = getIconPath("projectoroff");
    audioImageSource = getIconPath("speakeroff");
    lightImageSource = getIconPath("lightbulboff");
    // Connect signals and slots
    QObject::connect(&displayLogic, SIGNAL(statusChanged()), this, SLOT(displayStatusChanged()));
    QObject::connect(&audioLogic, SIGNAL(statusChanged()), this, SLOT(audioStatusChanged()));
    QObject::connect(&lightsLogic, SIGNAL(statusChanged()), this, SLOT(lightsStatusChanged()));
    connect(&displayLogic, SIGNAL(cmdMessage(QString)), this, SLOT(handleCommunication(QString)));
}

void ControlPageHandler::setDisplayImage(QString src) {
    displayImageSource = getIconPath(src);
    qDebug() << "ControlPageHandler::setDisplayImage() " << displayImageSource;
}

QString ControlPageHandler::getDisplayImage() {
    return displayImageSource;
}

void ControlPageHandler::setAudioImage(QString src) {
    audioImageSource = getIconPath(src);
}

QString ControlPageHandler::getAudioImage() {
    return audioImageSource;
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
    QString data = "Proj,Pwr,On";
    emit commandMessage(data);
    qDebug() << "ControlPageHandler::displayClicked()";
    displayLogic.onClicked();
}
void ControlPageHandler::audioClicked() {
    qDebug() << "ControlPageHandler::audioClicked()";
    audioLogic.onClicked();
}
void ControlPageHandler::lightsClicked(){
    qDebug() << "ControlPageHandler::lightsClicked()";
    lightsLogic.onClicked();
}

void ControlPageHandler::setCommunication(QObject *com) {
// TODO fix later, should be able to load different communication methods. Maybe as a list to allow different communication methods?
   qDebug() << "ControlPageHandler::setCommunication() for BLE";
   commHandler = com;
   // If ble
   connect(&displayLogic, SIGNAL(cmdMessage(QString)), qobject_cast<BLEHandler *>(commHandler), SLOT(transmitData(QString)));
};
// TODO move this somewhere else, this should not exist on page handler
void ControlPageHandler::handleCommunication(QString msg) {
    qDebug() << "ControlPageHandler::handleCommunication() " << msg;
    qobject_cast<BLEHandler *>(commHandler)->transmitData(msg);
}

// Slots
// QML property updates
void ControlPageHandler::displayStatusChanged() {
    qDebug() << "ControlPageHandler::dispalyStatusChanged()";
    // Check power state
    if(displayLogic.getPower()) setDisplayImage("projectoron");
    else setDisplayImage("projectoroff");
    // Tell QML element to update
    emit statusChanged();
}

void ControlPageHandler::audioStatusChanged() {
    qDebug() << "ControlPageHandler::dispalyStatusChanged()";
    // Check power state
    if(audioLogic.getPower()) setAudioImage("musicon");
    else setAudioImage("speakeroff");
    // Tell QML element to update
    emit statusChanged();
}

void ControlPageHandler::lightsStatusChanged() {
    qDebug() << "ControlPageHandler::dispalyStatusChanged()";
    // Check power state
    if(lightsLogic.getPower()) setLightImage("lightbulbon");
    else setLightImage("lightbulboff");
    // Tell QML element to update
    emit statusChanged();
}

QVariant ControlPageHandler::getDisplaySource() {
    qDebug() << "ControlPageHandler::getDisplaySource()";
    return  QVariant(displayImageSource);   // QML element requires var
}

QVariant ControlPageHandler::getAudioSource() {
    qDebug() << "ControlPageHandler::getDisplaySource()";
    return  QVariant(audioImageSource);   // QML element requires var
}

QVariant ControlPageHandler::getLightsSource() {
    qDebug() << "ControlPageHandler::getDisplaySource()";
    return  QVariant(lightImageSource);   // QML element requires var
}
