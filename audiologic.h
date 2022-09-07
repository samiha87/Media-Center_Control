#ifndef AUDIOCLASS_H
#define AUDIOCLASS_H
// Qt Includes
#include <QObject>
// Projector Includes
#include "devicelogic.h"

class AudioLogic : public QObject, DeviceLogic
{
    Q_OBJECT
public:
    explicit AudioLogic(QObject *parent = nullptr);
    // called when speaker icon is pressed for long time
    void onLongPress() override;
    // Called when speaker icon clicked
    void onClicked() override;
    void setPower(bool state) override;
    bool getPower() override;
    // Parse messag from Media Center and update status
    bool parseMessage(QByteArray msg);
    bool getMute();
    // Set volume level
    void setVolume(int vol);
    // Return volume
    void setMute(bool choice);
    int getVolume();
    void volumeUp();
    void volumeDown();

signals:
   void statusChanged();
   void cmdMessage(QString);

public slots:
private:
   bool powerState;
   bool audioMute;
   int audioVolume;
};

#endif // AUDIOCLASS_H
