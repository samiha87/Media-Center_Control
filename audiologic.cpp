// Qt Includes
#include <QDebug>
// Project Includes
#include "audiologic.h"

AudioLogic::AudioLogic(QObject *parent) : QObject(parent)
{
    powerState = false;
    audioMute = false;
}

void AudioLogic::onClicked() {
    setPower(!powerState);
}

void AudioLogic::setPower(bool state) {
    powerState = state;
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
        }

        if(msg.contains("Mute=On") && !audioMute) {
            audioMute = true;
            qDebug() << "AudioLogic::parseMessage() Audio mute on";
        }

        if(msg.contains("Mute=Off") && audioMute) {
            audioMute = false;
            qDebug() << "AudioLogic::parseMessage() Audio mute off";
        }
    }

    return change;
}
