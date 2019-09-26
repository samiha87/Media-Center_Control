// This class is virtual base for all device types.
#ifndef DEVICELOGIC_H
#define DEVICELOGIC_H

class DeviceLogic {
public:
    virtual ~DeviceLogic() {}
    virtual void onLongPress() = 0;
    virtual void onClicked() = 0;
    virtual bool getPower() = 0;
    virtual void setPower(bool state) = 0;
private:


};

#endif // DEVICELOGIC_H
