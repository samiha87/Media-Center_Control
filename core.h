#ifndef CORE_H
#define CORE_H
// Qt Includes
#include <QObject>
// Project includes
#include "controlpagehandler.h"
#include "playerpagehandler.h"
class Core : public QObject
{
    Q_OBJECT
public:
    explicit Core(QObject *parent = nullptr);
    // Initialize and start app
    void initialize();
signals:

public slots:
private:
    ControlPageHandler *controlPage;
    PlayerPageHandler *playerPage;

    void initIcons();
};

#endif // CORE_H
