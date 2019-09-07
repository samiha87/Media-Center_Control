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
    ~DisplayLogic();
    void onClicked() override;

    bool getPower() override;
    void setPower(bool state) override;

signals:

public slots:
};

#endif // DISPLAYLOGIC_H
