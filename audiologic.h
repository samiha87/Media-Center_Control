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
    void onClicked() override;
    void setPower(bool state) override;
    bool getPower() override;
    // Parse messag from Media Center and update status
    bool parseMessage(QByteArray msg);
    bool getMute();

signals:
   void statusChanged();

public slots:
private:
   bool powerState;
   bool audioMute;
};

#endif // AUDIOCLASS_H
