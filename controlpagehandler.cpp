#include <QtDebug>
#include "controlpagehandler.h"

ControlPageHandler::ControlPageHandler(QObject *parent) : ObjectHandler(parent)
{
    displayImageSource = getIconPath("projectoroff");
    audioImageSource = getIconPath("musicoff");
    lightImageSource = getIconPath("lightbulboff");
    // Connect signals and slots
    QObject::connect(&displayLogic, SIGNAL(statusChanged()), this, SLOT(displayStatusChanged()));
    QObject::connect(&audioLogic, SIGNAL(statusChanged()), this, SLOT(audioStatusChanged()));
    QObject::connect(&lightsLogic, SIGNAL(statusChanged()), this, SLOT(lightsStatusChanged()));
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
    else setAudioImage("musicoff");
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
