#include "objecthandler.h"

ObjectHandler::ObjectHandler(QObject *parent) : QObject(parent)
{

}

void ObjectHandler::setImageSource(QString src) {
    imageSource = src;
}

QString ObjectHandler::getImageSource() {
    return imageSource;
}
