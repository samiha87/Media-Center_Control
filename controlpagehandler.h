#ifndef CONTROLPAGEHANDLER_H
#define CONTROLPAGEHANDLER_H
// Qt Includes
#include <QObject>
// Project includes
#include "objecthandler.h"
#include "displaylogic.h"

class ControlPageHandler : public ObjectHandler
{
    Q_OBJECT
public:
    explicit ControlPageHandler(QObject *parent = nullptr);
    void setQMLObject(QObject *obj);

    // Set display image source
    void setDisplayImage(QString src);
    // Return display image source
    QString getDisplayImage();

    // Set music image source
    void setMusicImage(QString src);
    // Return music image source
    QString getMusicImage();

    // Set Llght image source
    void setLightImage(QString src);
    // Return light image source
    QString getLightImage();

    Q_INVOKABLE void displayClicked();
    Q_INVOKABLE void musicClicked();
    Q_INVOKABLE void lightsClicked();
signals:

public slots:

private:
    QString displayImageSource;
    QString musicImageSource;
    QString lightImageSource;

    DisplayLogic displayLogic;

    QString getIconPath(QString iconName);
};

#endif // CONTROLPAGEHANDLER_H
