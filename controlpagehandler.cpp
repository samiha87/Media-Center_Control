#include <QtDebug>
#include "controlpagehandler.h"

ControlPageHandler::ControlPageHandler(QObject *parent) : ObjectHandler(parent)
{
    displayImageSource = getIconPath("projectoroff");
    audioImageSource = getIconPath("speakeroff");
    lightImageSource = getIconPath("lightbulboff");
    statusMessage = "";
    bleHandler = new Device();
    // Connect signals and slots
    QObject::connect(&displayLogic, SIGNAL(statusChanged()), this, SLOT(displayStatusChanged()));
    QObject::connect(&audioLogic, SIGNAL(statusChanged()), this, SLOT(audioStatusChanged()));
    QObject::connect(&lightsLogic, SIGNAL(statusChanged()), this, SLOT(lightsStatusChanged()));
    connect(&displayLogic, SIGNAL(cmdMessage(QString)), this, SLOT(handleCommunication(QString)));

    connect(&displayLogic, SIGNAL(cmdMessage(QString)), bleHandler, SLOT(transmitData(QString)));
    connect(&audioLogic, SIGNAL(cmdMessage(QString)), bleHandler, SLOT(transmitData(QString)));
    connect(bleHandler, SIGNAL(updateStatus(QString)), this, SLOT(connectionStatus(QString)));
    connect(bleHandler, SIGNAL(messageReceived(QByteArray)), this, SLOT(messageReceived(QByteArray)));
    // Needsto becalled after connections are made
    bleHandler->startDeviceDiscovery();
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
   // emit commandMessage(data);
    qDebug() << "ControlPageHandler::displayClicked()";
    displayLogic.onClicked();
    processUI();
}
void ControlPageHandler::audioClicked() {
    qDebug() << "ControlPageHandler::audioClicked()";
    audioLogic.onClicked();
}
void ControlPageHandler::lightsClicked(){
    qDebug() << "ControlPageHandler::lightsClicked()";
    lightsLogic.onClicked();
}

void ControlPageHandler::volumeUpClicked(){
    qDebug() << "ControlPageHandler::lightsClicked()";
    audioLogic.volumeUp();
}

void ControlPageHandler::volumeDownClicked(){
    qDebug() << "ControlPageHandler::lightsClicked()";
    audioLogic.volumeDown();
}

void ControlPageHandler::displayLongPressed() {
    QString data = "Proj,Pwr,On";
   // emit commandMessage(data);
    qDebug() << "ControlPageHandler::displayClicked()";
    displayLogic.onLongPress();
    // Ensure audio stops
    if(displayLogic.getPower()) audioLogic.setMute(false);
    else audioLogic.setMute(true);

    processUI();
}
void ControlPageHandler::audioLongPressed() {
    //qDebug() << "ControlPageHandler::audioClicked()";
    audioLogic.onLongPress();
    if(audioLogic.getPower()) audioLogic.setMute(false);
    else audioLogic.setMute(true);
}
void ControlPageHandler::lightsLongPressed(){
    //qDebug() << "ControlPageHandler::lightsClicked()";
    lightsLogic.onLongPress();
}

void ControlPageHandler::setCommunication(QObject *com) {
// TODO fix later, should be able to load different communication methods. Maybe as a list to allow different communication methods?
   qDebug() << "ControlPageHandler::setCommunication() for BLE";
   commHandler = com;
   // If ble
   qobject_cast<Device *>(commHandler)->startDeviceDiscovery();
};

// TODO move this somewhere else, this should not exist on page handler
void ControlPageHandler::handleCommunication(QString msg) {
    qDebug() << "ControlPageHandler::handleCommunication() " << msg;
    bleHandler->transmitData(msg);
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
    if(audioLogic.getPower()) {
        if(!audioLogic.getMute()) {
            setAudioImage("musicon");
            if(audioLogic.getVolume() == 0) {
                setAudioImage("speakerlvl0");
            } else if (audioLogic.getVolume() < 20) {
                setAudioImage("speakerlvl1");
            } else if (audioLogic.getVolume() < 40) {
                setAudioImage("speakerlvl2");
            } else if (audioLogic.getVolume() < 60) {
                setAudioImage("speakerlvl3");
            } else if (audioLogic.getVolume() < 80) {
                setAudioImage("speakerlvl4");
            } else if (audioLogic.getVolume() == 100) {
                setAudioImage("speakerlvl5");
            }
         } else setAudioImage("speakermute");
    } else setAudioImage("speakeroff");
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
    qDebug() << "ControlPageHandler::getAudioSource()";
    return  QVariant(audioImageSource);   // QML element requires var
}

QVariant ControlPageHandler::getLightsSource() {
    qDebug() << "ControlPageHandler::getLightsSource()";
    return  QVariant(lightImageSource);   // QML element requires var
}

QVariant ControlPageHandler::getBleConnected() {
    return QVariant(bleHandler->isConnected());
}

void ControlPageHandler::connectionStatus(QString msg) {
    qDebug() << "Message"<< msg;
    statusMessage = msg;
    // Update qml
    if(statusMessage.contains("connected")) {
        emit statusChanged();
        emit activate();
    }
    emit textChanged();
}

QVariant ControlPageHandler::getStatusText() {
    return statusMessage;
}

QVariant ControlPageHandler::getVolumeActive() {
    return audioLogic.getPower();
}

void ControlPageHandler::messageReceived(QByteArray msg) {
    qDebug() << "ControlPageHandler::messageReceived() "<< msg;
    // Parse message
    // Check if anything has changed. If properties are same do not update UI
    bool changeDisplay, changeAudio, changeLights;
    changeDisplay = changeAudio = changeLights= false;

    if(msg.contains("Proj") || msg.contains("TV")) changeDisplay = displayLogic.parseMessage(msg);
    if(msg.contains("Audio")) {
      changeAudio = audioLogic.parseMessage(msg);
      audioStatusChanged();
    }
    if(msg.contains("Lights"))  changeLights = lightsLogic.parseMessage(msg);
    if(changeDisplay || changeAudio || changeLights) processUI();
}

void ControlPageHandler::processUI() {

    // If display is off
    if(!displayLogic.getPower()) {
        if(getDisplayImage().contains("projectoron")) setDisplayImage("projectoroff");
        // Depending on settings in media center instance if display not powered all choices are set off
        // If receives message that doesn't match requested state will send command to shutdown or start depending on the current state of control
        if(audioLogic.getPower()) {
           audioLogic.setPower(false);
        }

        if(lightsLogic.getPower()) {
            lightsLogic.setPower(false);
        }

        return;
    }
    // If display is on, we assume if projector is on audio will also start
    if(displayLogic.getPower()) {
        if(getDisplayImage().contains("projectoroff")) setDisplayImage("projectoron");
        // If audio powe is on QML activates volume adjustment
        if(!audioLogic.getPower()) audioLogic.setPower(true);
    }
    emit statusChanged();
}
