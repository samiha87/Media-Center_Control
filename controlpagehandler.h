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
#include "device.h"

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
    // Click events
    // Pop up a matrix menu, lets user chose what wants to display
    // If user have a matrix 2HDMI input and 1 hdmi output. Nintendo is on input 1. Pressing Nintendo
    // from input menut drives input 1 to output 1 and displays nintedo on dispaly.
    Q_INVOKABLE void displayClicked();
    // Set mute on or off
    Q_INVOKABLE void audioClicked();
    // Popup a lights preset menu
    Q_INVOKABLE void lightsClicked();
    // Adjust volume up
    Q_INVOKABLE void volumeUpClicked();
    // Adjust volume down
    Q_INVOKABLE void volumeDownClicked();

    // Press and hold events
    // Turn on or off display
    Q_INVOKABLE void displayLongPressed();
    // Turn on or off audio
    Q_INVOKABLE void audioLongPressed();
    // Turn on or off master lights
    Q_INVOKABLE void lightsLongPressed();

    void setCommunication(QObject *com);
signals:
    // Emitting this signal tells QML element to update image properties
    void statusChanged();
    void activate();
    void textChanged();
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
    // Return bluetooth connection status
    QVariant getBleConnected();
    void handleCommunication(QString msg);
    // Allows qml to get status text
    QVariant getStatusText();
    // Determines wether volume adjustment arrow buttons are visible for user
    QVariant getVolumeActive();
private:
    QString displayImageSource;
    QString audioImageSource;
    QString lightImageSource;

    DisplayLogic displayLogic;
    AudioLogic   audioLogic;
    LightsLogic  lightsLogic;
    QObject *commHandler;
    Device *bleHandler;
    QString statusMessage;

    QString getIconPath(QString iconName);
    void processUI();
private slots:
    void connectionStatus(QString msg);
    void messageReceived(QByteArray msg);


};

#endif // CONTROLPAGEHANDLER_H
