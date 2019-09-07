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
}
