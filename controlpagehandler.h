#ifndef CONTROLPAGEHANDLER_H
#define CONTROLPAGEHANDLER_H
// Qt Includes
#include <QObject>
// Project includes
#include "objecthandler.h"
class ControlPageHandler : public ObjectHandler
{
    Q_OBJECT
public:
    explicit ControlPageHandler(QObject *parent = nullptr);
    void setQMLObject(QObject *obj);
signals:

public slots:
};

#endif // CONTROLPAGEHANDLER_H
