// Qt Includes
#include <QDebug>
// Project Includes
#include "displaylogic.h"

DisplayLogic::DisplayLogic(QObject *parent) : QObject(parent)
{

}

DisplayLogic::~DisplayLogic() {

}

void DisplayLogic::onClicked() {
    qDebug() << "DisplayLogic::onClicked()";
    if(powerState) powerState= false;
    else powerState = true;
    setPower(powerState);
    if(powerState)qDebug() << "DisplayLogic::onClicked() display is on";
    else qDebug() << "DisplayLogic::onClicked() display is off";
}

void DisplayLogic::setPower(bool state) {
    powerState = state;
    emit statusChanged();
    if(powerState) emit cmdMessage("PWR:ON");
    else emit cmdMessage("PWR:OFF");

}

bool DisplayLogic::getPower() {
    return powerState;
}

