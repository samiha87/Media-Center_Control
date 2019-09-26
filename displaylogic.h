/* This class contains logic for any display device. Settings, controls, status and etc
 */
#ifndef DISPLAYLOGIC_H
#define DISPLAYLOGIC_H

#include <QObject>
// Virtual base
#include "devicelogic.h"

class DisplayLogic : public QObject, public DeviceLogic
{
    Q_OBJECT
public:
    explicit DisplayLogic(QObject *parent = nullptr);
    ~DisplayLogic() override;
    // Turn display on or off
    void onLongPress() override;
    // Control page icon clicked event
    void onClicked() override;
    // Return power of current device
    bool getPower() override;
    // Set power for current device
    void setPower(bool state) override;
    // Parse messag from Media Center and update status
    bool parseMessage(QByteArray msg);

signals:
    void statusChanged();
    void cmdMessage(QString);
public slots:
private:
    bool requestedPowerState;
    bool powerState;
    int lampHours;
};

#endif // DISPLAYLOGIC_H
