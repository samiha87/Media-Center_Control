// QtIncludes
#include <qbluetoothaddress.h>
#include <qbluetoothdevicediscoveryagent.h>
#include <qbluetoothlocaldevice.h>
#include <qbluetoothdeviceinfo.h>
#include <qbluetoothservicediscoveryagent.h>
#include <QDebug>
#include <QList>
#include <QTimer>

#include "blehandler.h"
#include "deviceinfo.h"

BLEHandler::BLEHandler(QObject *parent) : QObject(parent)
{
    transmitService = nullptr;
    controller = nullptr;
    discoveryAgent = new QBluetoothDeviceDiscoveryAgent();

    // Connects
    connect(discoveryAgent, SIGNAL(deviceDiscovered(const QBluetoothDeviceInfo&)), this, SLOT(addDevice(const QBluetoothDeviceInfo&)));
    connect(discoveryAgent, SIGNAL(error(QBluetoothDeviceDiscoveryAgent::Error)),this, SLOT(deviceScanError(QBluetoothDeviceDiscoveryAgent::Error)));
    connect(discoveryAgent, SIGNAL(finished()), this, SLOT(deviceScanFinished()));
}

BLEHandler::~BLEHandler() {

}

void BLEHandler::scanDevices() {
    qDebug() << "BLEHandler::scanDevices() started";
    discoveryAgent->start();
}

void BLEHandler::addDevice(const QBluetoothDeviceInfo &info)
{
    qDebug() << "BLEHandler::addDevice()";
    if (info.coreConfigurations() & QBluetoothDeviceInfo::LowEnergyCoreConfiguration) {
        DeviceInfo *d = new DeviceInfo(info);
        // Find correct device
        if(d->getName().contains("Totem_Blue")) {
            qDebug() << "BLEHandler::addDevice() Device found";
            instanceDevice = d;
            discoveryAgent->stop();
            // find transmitservice
            findController(d);
        }
    }
}

void BLEHandler::findController(DeviceInfo *dev) {
    qDebug() << "BLEHandler::findTransmitService";
    if(dev == nullptr)  {
        qDebug() << "BLEHandler::findTrasmitService() Device not initialized";
        return;
    }
    if(!dev->getDevice().isValid()) {
        delete dev;
        qDebug() << "BLEHandler::findTransmitService() Device not valid";
    }
    // If controller exist, disconnect it from previous device and delete controller
    if(controller != nullptr) {
        qDebug() << "BLEHandler::findTransmitService() Controller already exist";
        controller->disconnectFromDevice();
        // Disconnect made connections
        disconnect(controller, SIGNAL(connected()),this, SLOT(deviceConnected()));
        disconnect(controller, SIGNAL(error(QLowEnergyController::Error)), this, SLOT(errorReceived(QLowEnergyController::Error)));
        disconnect(controller, SIGNAL(disconnected()), this, SLOT(deviceDisconnected()));
        disconnect(controller, SIGNAL(serviceDiscovered(QBluetoothUuid)), this, SLOT(addLowEnergyService(QBluetoothUuid)));
        disconnect(controller, SIGNAL(discoveryFinished()),  this, SLOT(serviceScanDone()));
        delete controller;
    }
    if(dev != nullptr) {
        qDebug() << "BLEHandler::findTransmitService() Create controller and conenct to device ";
        controller = new QLowEnergyController(dev->getDevice());
        controller->connectToDevice();
        connect(controller, SIGNAL(connected()),this, SLOT(deviceConnected()));
        connect(controller, SIGNAL(error(QLowEnergyController::Error)), this, SLOT(errorReceived(QLowEnergyController::Error)));
        connect(controller, SIGNAL(disconnected()), this, SLOT(deviceDisconnected()));
        connect(controller, SIGNAL(serviceDiscovered(QBluetoothUuid)), this, SLOT(addLowEnergyService(QBluetoothUuid)));
        connect(controller, SIGNAL(discoveryFinished()),  this, SLOT(serviceScanDone()));
    }
}

void BLEHandler::addLowEnergyService(const QBluetoothUuid &serviceUuid)
{
    qDebug() << "BLEHandler::addLowEnergyService(): Begin";
    QLowEnergyService *service = controller->createServiceObject(serviceUuid);
    if (!service) {
        qWarning() << "BLEHandler::addLowEnergyService() Cannot create service for uuid";
        return;
    }
    ServiceInfo *serv = new ServiceInfo(service);
    qDebug() << "BLEHandler::addLowEnergyService() Adding new ble service: "<< serv->getUuid();

    if(serv->isTransfer()) {
        qDebug() << "BLEHandler::addLowEnergyService() Transmit service found";
        transmitService = service;
        connectToService();
    }
}

void BLEHandler::serviceScanDone()
{
    qDebug() << "BLEHandler::serviceScanDone()";
}

void BLEHandler::connectToService()
{
    qDebug() << "BLEHandler::connectToService()";
    if (transmitService->state() == QLowEnergyService::DiscoveryRequired) {
        connect(transmitService, SIGNAL(stateChanged(QLowEnergyService::ServiceState)), this, SLOT(serviceDetailsDiscovered(QLowEnergyService::ServiceState)));
        transmitService->discoverDetails();
        return;
    }
    //discovery already done
}

void BLEHandler::deviceConnected()
{
    qDebug() << "BLEHandler::deviceConnected()";
    controller->discoverServices();
}

void BLEHandler::deviceDisconnected()
{
   qDebug() << "BLEHandler::deviceDisconnected()";
}

void BLEHandler::errorReceived(QLowEnergyController::Error /*error*/)
{
    qWarning() << "Error: " << controller->errorString();
}

void BLEHandler::serviceDetailsDiscovered(QLowEnergyService::ServiceState newState)
{
    if (newState != QLowEnergyService::ServiceDiscovered) {
        // do not hang in "Scanning for characteristics" mode forever
        // in case the service discovery failed
        // We have to queue the signal up to give UI time to even enter
        // the above mode
        if (newState != QLowEnergyService::DiscoveringServices) {
            QMetaObject::invokeMethod(this, "characteristicsUpdated", Qt::QueuedConnection);
        }
        return;
    }

    QLowEnergyService *service = qobject_cast<QLowEnergyService *>(sender());
    if (!service)
        return;

    const QList<QLowEnergyCharacteristic> chars = service->characteristics();

    foreach (QLowEnergyCharacteristic characteristic, transmitService->characteristics()) {
        QLowEnergyDescriptor m_notificationDesc = characteristic.descriptor(QBluetoothUuid::ClientCharacteristicConfiguration);
        transmitService->writeDescriptor(m_notificationDesc, QByteArray::fromHex("0100"));  // Enable notifications
        connect(transmitService, SIGNAL(stateChanged(QLowEnergyService::ServiceState)), this, SLOT(serialStateChanged(QLowEnergyService::ServiceState)));
        connect(transmitService, SIGNAL(characteristicChanged(QLowEnergyCharacteristic, QByteArray)), this, SLOT(serialReadValue(QLowEnergyCharacteristic,QByteArray)));
        connect(transmitService, SIGNAL(descriptorWritten(QLowEnergyDescriptor, QByteArray)), this, SLOT(serialDescriptorWrite(QLowEnergyDescriptor,QByteArray)));
    }
}

void BLEHandler::serialStateChanged(QLowEnergyService::ServiceState s)
{
    //qDebug() << "Serial state changed";
    switch (s) {
    case QLowEnergyService::ServiceDiscovered:
    {
        const QLowEnergyCharacteristic hrChar = transmitService->characteristics().at(transmitPointer);
        if (!hrChar.isValid()) {
            qDebug() << "Data not found";
            break;
        }

        const QLowEnergyDescriptor m_notificationDesc = hrChar.descriptor(
                    QBluetoothUuid::ClientCharacteristicConfiguration);
        if (m_notificationDesc.isValid()) {
            transmitService->writeDescriptor(m_notificationDesc, QByteArray::fromHex("0100"));
           // qDebug() << "Receiving enabled";
        }

        break;
    }
    default:
        //nothing for now
        break;
    }
}

void BLEHandler::serialReadValue(const QLowEnergyCharacteristic &c, const QByteArray &value)
{
    // ignore any other characteristic change -> shouldn't really happen though
    qDebug() << "Device::Serial read value: " << value;
    if (c.uuid().toString().contains("0000ffe0"))
        return;

}

void BLEHandler::serialDescriptorWrite(const QLowEnergyDescriptor &d,
                                         const QByteArray &value)
{
    // Disconnect from device
    if(transmitService == nullptr) {
        qDebug() << "BLEHandler::serialDescriptorWrite() transmit service doesn't exist";
        return;
    }
    if(transmitService->characteristics().count() < transmitPointer) {
        qDebug() << "BLEHandler::serialDescriptorWrite() Transmit pointer is higher than characteristic count";
        return;
    }
    const QLowEnergyCharacteristic hrChar = transmitService->characteristics().at(transmitPointer);

    if (!hrChar.isValid()) {
        return;
    }

    QLowEnergyDescriptor m_notificationDesc = hrChar.descriptor(QBluetoothUuid::ClientCharacteristicConfiguration);

    if (d.isValid() && d == m_notificationDesc && value == QByteArray("0000")) {
        //disabled notifications -> assume disconnect intent
        controller->disconnectFromDevice();
        if(transmitService != nullptr) {
            delete transmitService;
            transmitService = nullptr;
            transmitPointer = 0;
        }
    }
}

void BLEHandler::deviceScanFinished()
{
    qDebug() << "BLEHandler::deviceScanFinished()";
    discoveryAgent->stop();
}

void BLEHandler::deviceScanError(QBluetoothDeviceDiscoveryAgent::Error error)
{
    if (error == QBluetoothDeviceDiscoveryAgent::PoweredOffError)
        qDebug() << "BLEHandler::deviceScanError() " << "The Bluetooth adaptor is powered off, power it on before doing discovery.";
    else if (error == QBluetoothDeviceDiscoveryAgent::InputOutputError)
        qDebug() << "BLEHandler::deviceScanError" << "Writing or reading from the device resulted in an error.";
    else
        qDebug() << "BLEHandler::deviceScanError" << "An unknown error has occurred.";

}

void BLEHandler::transmitData(QString cmd)
{
    qDebug() << "Device::transmitData() ";
   /* if(transmitService == nullptr) {
        qDebug() << "transmitService not found";
        return;
    }*/
    //transmitService->writeCharacteristic(transmitService->characteristics().at(transmitPointer), msg, QLowEnergyService::WriteWithoutResponse);
}
