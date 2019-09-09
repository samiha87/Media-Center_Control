#ifndef CONTROLPAGEHANDLER_H
#define CONTROLPAGEHANDLER_H
// Qt Includes
#include <QObject>
#include <QVariant>
// Project includes
#include "objecthandler.h"
#include "displaylogic.h"
#include "audiologic.h"
#include "lightslogic.h"
#include "blehandler.h"

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
    void setAudioImage(QString src);
    // Return music image source
    QString getAudioImage();

    // Set Llght image source
    void setLightImage(QString src);
    // Return light image source
    QString getLightImage();

    Q_INVOKABLE void displayClicked();
    Q_INVOKABLE void audioClicked();
    Q_INVOKABLE void lightsClicked();

    void setCommunication(QObject *com);
signals:
    // Emitting this signal tells QML element to update image properties
    void statusChanged();
    void commandMessage(QString message);

public slots:
    // When device status changed one of the following need to be called
    // Updates display properties on QML element
    void displayStatusChanged();
    // Updates music properties on QML element
    void audioStatusChanged();
    // Updates lights properties on QML element
    void lightsStatusChanged();
    // Return display image source
    QVariant getDisplaySource();
    // Return display image source
    QVariant getAudioSource();
    // Return display image source
    QVariant getLightsSource();
    void handleCommunication(QString msg);

private:
    QString displayImageSource;
    QString audioImageSource;
    QString lightImageSource;

    DisplayLogic displayLogic;
    AudioLogic   audioLogic;
    LightsLogic  lightsLogic;
    QObject *commHandler;

    QString getIconPath(QString iconName);


};

#endif // CONTROLPAGEHANDLER_H
