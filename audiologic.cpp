#include "audiologic.h"

AudioLogic::AudioLogic(QObject *parent) : QObject(parent)
{
    powerState = false;
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
