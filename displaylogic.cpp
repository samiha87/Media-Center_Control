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
    emit statusChanged();
}

bool DisplayLogic::getPower() {
    return powerState;
}

