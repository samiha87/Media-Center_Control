// Qt Includes
#include <QDebug>
// Project Includes
#include "audiologic.h"

AudioLogic::AudioLogic(QObject *parent) : QObject(parent)
{
    powerState = false;
    audioMute = false;
    audioVolume = 0;
}

void AudioLogic::onClicked() {
    setPower(!powerState);
}

void AudioLogic::setPower(bool state) {
    powerState = state;
    if(powerState) emit cmdMessage("Audio,Pwr,On");
    else emit cmdMessage("Audio,Pwr,Off");
    emit statusChanged();
}

bool AudioLogic::getPower(){
    return powerState;
}

bool AudioLogic::parseMessage(QByteArray msg) {
    bool change = false;
    if(!msg.contains("Audio")) {
        return false;
    }

    if(msg.contains("Vol")) {
        if(msg.contains("Level")) {
            // Add to index Level=
            int start = msg.indexOf("Level") + 6;
            int end = msg.indexOf(",", start);
            QByteArray parsed = msg.mid(start, (end - start));
            qDebug() << "AudioLogic::parseMessage() " << parsed;
            audioVolume = parsed.toInt();
        }

        if(msg.contains("Mute=On") && !audioMute) {
            audioMute = true;
            qDebug() << "AudioLogic::parseMessage() Audio mute on";
            change = true;
        }

        if(msg.contains("Mute=Off") && audioMute) {
            audioMute = false;
            qDebug() << "AudioLogic::parseMessage() Audio mute off";
            change = true;
        }
    }
    return change;
}

void AudioLogic::setMute(bool choice) {
    if(choice) emit cmdMessage("Audio,Vol,Mute,On");
    else emit cmdMessage("Audio,Vol,Mute,Off");
    audioMute = choice;
}

bool AudioLogic::getMute() {
    return audioMute;
}

int AudioLogic::getVolume() {
    return audioVolume;
}

void AudioLogic::volumeUp() {
    emit cmdMessage("Audio,Vol,Up");
}

void AudioLogic::volumeDown() {
    emit cmdMessage("Audio,Vol,Down");
}

void AudioLogic::setVolume(int vol) {
    if (vol < 0) vol = 0;
    if (vol > 100) vol = 100;
    audioVolume = vol;
}
