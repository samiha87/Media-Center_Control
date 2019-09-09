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
    // Control page icon clicked event
    void onClicked() override;
    // Return power of current device
    bool getPower() override;
    // Set power for current device
    void setPower(bool state) override;

signals:
    void statusChanged();
    void cmdMessage(QString);
public slots:
private:
    bool powerState;
};

#endif // DISPLAYLOGIC_H
