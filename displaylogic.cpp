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
    setPower(!powerState);
}

void DisplayLogic::setPower(bool state) {
    powerState = state;
    if(powerState) emit cmdMessage("PWR:ON");
    else emit cmdMessage("PWR:OFF");
    emit statusChanged();
}

bool DisplayLogic::getPower() {
    return powerState;
}

