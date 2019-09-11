#ifndef LIGHTSLOGIC_H
#define LIGHTSLOGIC_H
// Qt Includes
#include <QObject>
// Project Includes
#include "devicelogic.h"
class LightsLogic : public QObject, DeviceLogic
{
    Q_OBJECT
public:
    explicit LightsLogic(QObject *parent = nullptr);
    // Toggle power state
    void onClicked() override;
    // Set power status
    void setPower(bool state) override;
    // Get power status
    bool getPower() override;
    // Parse messag from Media Center and update status
    bool parseMessage(QByteArray msg);
signals:
    void statusChanged();

public slots:
private:
    bool powerState;
};

#endif // LIGHTSLOGIC_H
