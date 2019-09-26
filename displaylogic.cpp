// Qt Includes
#include <QDebug>
// Project Includes
#include "displaylogic.h"

DisplayLogic::DisplayLogic(QObject *parent) : QObject(parent)
{
    lampHours = 0;
    requestedPowerState = false;
}

DisplayLogic::~DisplayLogic() {

}

void DisplayLogic::onLongPress() {
    qDebug() << "DisplayLogic::onClicked()";
    if(powerState) powerState= false;
    else powerState = true;
    setPower(powerState);
    if(powerState)qDebug() << "DisplayLogic::onClicked() display is on";
    else qDebug() << "DisplayLogic::onClicked() display is off";
}

void DisplayLogic::onClicked() {
    qDebug() << "DisplayLogic::onClicked()";
 }

void DisplayLogic::setPower(bool state) {
    powerState = state;
    requestedPowerState = state;
    emit statusChanged();
    if(powerState) emit cmdMessage("Proj,Pwr,On");
    else emit cmdMessage("Proj,Pwr,Off");
}

bool DisplayLogic::getPower() {
    return powerState;
}

// Parse messag from Media Center and update status
bool DisplayLogic::parseMessage(QByteArray msg) {
    // No display related message received
    bool change = false;
    if(!msg.contains("Proj")) return false;
    if(msg.contains("Pwr,1")) {
        powerState = true;

        qDebug() << "DisplayLogic::parseMessage() Power on ";
        change = true;
    }
    if(msg.contains("Pwr,0")) {
        qDebug() << "DisplayLogic::parseMessage() Power off ";
        powerState = false;
        change = true;
    }
    // Ensure powerstate will be as user requested
    if(powerState != requestedPowerState) {
        setPower(requestedPowerState);
    }
    if(msg.contains("Lamp") || msg.contains("lamp")) {
        int index = msg.indexOf("lamp");
        index = index + 5;
        msg.remove(0, index);
        lampHours = msg.toInt();
        change = true;
        qDebug() << "DisplayLogic::parseMessage() Lamp " << QString::number(lampHours);
    }
    return change;
}

