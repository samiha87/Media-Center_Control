#ifndef CORE_H
#define CORE_H
// Qt Includes
#include <QObject>
// Project includes
#include "controlpagehandler.h"
#include "playerpagehandler.h"
#include "blehandler.h"

class Core : public QObject
{
    Q_OBJECT
public:
    explicit Core(QObject *parent = nullptr);
    // Initialize and start app
    void initialize();
signals:

private:
    ControlPageHandler *controlPage;
    PlayerPageHandler *playerPage;
    BLEHandler *bleHandler;

    void initIcons();
    void checkPermissions();

};

#endif // CORE_H
