#ifndef BLEHANDLER_H
#define BLEHANDLER_H

#include <QObject>
// Qt Includes
#include <qbluetoothglobal.h>
#include <qbluetoothlocaldevice.h>
#include <QObject>
#include <QVariant>
#include <QTimer>
#include <QList>
#include <QBluetoothServiceDiscoveryAgent>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QLowEnergyController>
#include <QBluetoothServiceInfo>
// Project Includes
#include "deviceinfo.h"
#include "serviceinfo.h"
#include "characteristicinfo.h"

class BLEHandler : public QObject
{
    Q_OBJECT
public:
    explicit BLEHandler(QObject *parent = nullptr);
    ~BLEHandler();
    void scanDevices();
    void findController(DeviceInfo *dev);

signals:

public slots:
    void transmitData(QString cmd);
    void connectToService();
private:
    QLowEnergyService *transmitService;
    QBluetoothDeviceDiscoveryAgent *discoveryAgent;
    DeviceInfo *instanceDevice;
    QLowEnergyController *controller;
    int transmitPointer;

private slots:
    // QBluetoothDeviceDiscoveryAgent related
    void addDevice(const QBluetoothDeviceInfo&);
    void deviceScanFinished();
    void deviceScanError(QBluetoothDeviceDiscoveryAgent::Error);

    // QLowEnergyController realted
    void addLowEnergyService(const QBluetoothUuid &uuid);
    void deviceConnected();
    void errorReceived(QLowEnergyController::Error);
    void serviceScanDone();
    void deviceDisconnected();

    // QLowEnergyService related
    void serviceDetailsDiscovered(QLowEnergyService::ServiceState newState);
    void serialReadValue(const QLowEnergyCharacteristic &c, const QByteArray &value);
    void serialDescriptorWrite(const QLowEnergyDescriptor &d, const QByteArray &value);
    void serialStateChanged(QLowEnergyService::ServiceState s);
};

#endif // BLEHANDLER_H
