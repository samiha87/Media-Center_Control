#include "lightslogic.h"

LightsLogic::LightsLogic(QObject *parent) : QObject(parent)
{
    powerState = false;
}

void LightsLogic::onClicked(){
    setPower(!powerState);
}

void LightsLogic::setPower(bool state) {
    powerState = state;
    emit statusChanged();
}

bool LightsLogic::getPower() {
    return powerState;
}

bool LightsLogic::parseMessage(QByteArray msg) {
    bool change = false;
    return change;
}
