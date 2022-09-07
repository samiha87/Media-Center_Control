/***************************************************************************
**
** Copyright (C) 2013 BlackBerry Limited. All rights reserved.
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtBluetooth module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/


#include <qbluetoothaddress.h>
#include <qbluetoothdevicediscoveryagent.h>
#include <qbluetoothlocaldevice.h>
#include <qbluetoothdeviceinfo.h>
#include <qbluetoothservicediscoveryagent.h>
#include <QDebug>
#include <QList>
#include <QTimer>

#include "device.h"


Device::Device():connected(false), controller(nullptr), m_deviceScanState(false), randomAddress(false) {
    discoveryAgent = new QBluetoothDeviceDiscoveryAgent();
    connect(discoveryAgent, SIGNAL(deviceDiscovered(const QBluetoothDeviceInfo&)), this, SLOT(addDevice(const QBluetoothDeviceInfo&)));
    connect(discoveryAgent, SIGNAL(error(QBluetoothDeviceDiscoveryAgent::Error)), this, SLOT(deviceScanError(QBluetoothDeviceDiscoveryAgent::Error)));
    connect(discoveryAgent, SIGNAL(finished()), this, SLOT(deviceScanFinished()));

    connect(&quickSearchTimer, SIGNAL(timeout()),this, SLOT(deviceScanFinished()));
    tx = nullptr;
    transmitService = nullptr;
    transmitPointer = 0;
    searchParams = null;
    deviceVisuallyConnected = false;
}

Device::~Device()
{
    delete discoveryAgent;
    delete controller;
    qDeleteAll(devices);
    qDeleteAll(m_services);
    qDeleteAll(m_characteristics);
    devices.clear();
    m_services.clear();
    m_characteristics.clear();
}

void Device::startDeviceDiscovery()
{
    qDebug() << "Device::StartDeviceDiscovery()";
    updateStatus("Connecting bluetooth");
    qDeleteAll(devices);
    qDeleteAll(m_services);
    qDeleteAll(m_characteristics);
    devices.clear();
    m_services.clear();
    m_characteristics.clear();

    quickSearchTimer.start(20000);
    discoveryAgent->start();

    if (discoveryAgent->isActive()) {
        m_deviceScanState = true;
        Q_EMIT qstateChanged();
    }
}

void Device::addDevice(const QBluetoothDeviceInfo &info)
{
    qDebug() << "Device::addDevice() ";
    if (info.coreConfigurations() & QBluetoothDeviceInfo::LowEnergyCoreConfiguration) {
        DeviceInfo *d = new DeviceInfo(info);

        if(d->getName().startsWith("Media_Center") ) {
            qDebug() << "Device::addDevice() " << d->getName();
            // Check that we don't already contain this
            for(int i = 0; i < devices.count(); ++i) {
                if(qobject_cast<DeviceInfo *>(devices.value(i))->getAddress() == d->getAddress()){
                    return;
                }
            }
            qDebug() << "Device::addDevice() Found media center";
            devices.append(d);
            // If we found ble device, no need to scan. Stops scanning other ble devices
            discoveryAgent->stop();
            emit devicesUpdated();
        }
    }
}

void Device::deviceScanFinished()
{
    qDebug() << "Device::deviceScanFinished()";
    emit devicesUpdated();
    quickSearchTimer.stop();
    m_deviceScanState = false;
    discoveryAgent->stop();
    emit qstateChanged();
    // Start connecting to device
    if(devices.count() == 0) {
        qDebug() << "Device::deviceScanFinished() No devices found";
        startDeviceDiscovery();
        return;
    }
    DeviceInfo *d = qobject_cast<DeviceInfo *>(devices.first());
    scanServices(d->getAddress());
}

QVariant Device::getDevices()
{
    return QVariant::fromValue(devices);
}

QVariant Device::getServices()
{
    return QVariant::fromValue(m_services);
}

QVariant Device::getCharacteristics()
{
    return QVariant::fromValue(m_characteristics);
}

QString Device::getUpdate()
{
    return m_message;
}

void Device::scanServices(const QString &address)
{
    // We need the current device for service discovery.
    qDebug() << "Device::scanServices(): begin";
    for (int i = 0; i < devices.size(); i++) {
        if ((qobject_cast<DeviceInfo*>(devices.at(i)))->getAddress() == address )
            currentDevice.setDevice((qobject_cast<DeviceInfo*>(devices.at(i)))->getDevice());
    }

    if (!currentDevice.getDevice().isValid()) {
        qWarning() << "Not a valid device";
        return;
    }

    qDeleteAll(m_characteristics);
    m_characteristics.clear();
    emit characteristicsUpdated();
    qDeleteAll(m_services);
    m_services.clear();
    emit servicesUpdated();

    //if(controller == 0) return;

    if (controller && m_previousAddress != currentDevice.getAddress()) {
        controller->disconnectFromDevice();
        delete controller;
        controller = nullptr;
    }
    if (!controller) {

        controller = new QLowEnergyController(currentDevice.getDevice());
        connect(controller, SIGNAL(connected()), this, SLOT(deviceConnected()));
        connect(controller, SIGNAL(error(QLowEnergyController::Error)), this, SLOT(errorReceived(QLowEnergyController::Error)));
        connect(controller, SIGNAL(disconnected()), this, SLOT(deviceDisconnected()));
        connect(controller, SIGNAL(serviceDiscovered(QBluetoothUuid)), this, SLOT(addLowEnergyService(QBluetoothUuid)));
        connect(controller, SIGNAL(discoveryFinished()), this, SLOT(serviceScanDone()));
    }

    if (isRandomAddress())
        controller->setRemoteAddressType(QLowEnergyController::RandomAddress);
    else
        controller->setRemoteAddressType(QLowEnergyController::PublicAddress);
    controller->connectToDevice();
    m_previousAddress = currentDevice.getAddress();
    qDebug() << "Device::scanServices(): end";
}

void Device::addLowEnergyService(const QBluetoothUuid &serviceUuid)
{
    qDebug() << "Device::addLowEnergyService(): Begin";
    QLowEnergyService *service = controller->createServiceObject(serviceUuid);
    if (!service) {
        qWarning() << "Cannot create service for uuid";
        return;
    }
    ServiceInfo *serv = new ServiceInfo(service);
    qDebug() << "Adding new ble service: "<< serv->getUuid();
    for(int i = 0; i < m_services.count(); ++i) if(qobject_cast<ServiceInfo *>(m_services.value(i))->getUuid() == serv->getUuid()) return;
    m_services.append(serv);
    if(serv->isTransfer()) {
        // Search characteristics and connect
        connectToService(serv->getUuid());
    }
    emit servicesUpdated();
    qDebug() << "Device::addLowEnergyService(): Stop";
}

void Device::serviceScanDone()
{
    // force UI in case we didn't find anything
    if (m_services.isEmpty()) emit servicesUpdated();
}

void Device::connectToService(const QString &uuid)
{
    QLowEnergyService *service = nullptr;
    for (int i = 0; i < m_services.size(); i++) {
        ServiceInfo *serviceInfo = qobject_cast<ServiceInfo*>(m_services.at(i));
        if (serviceInfo->getUuid() == uuid) {
            service = serviceInfo->service();
            break;
        }
    }

    if (!service) return;

    qDeleteAll(m_characteristics);
    m_characteristics.clear();
    emit characteristicsUpdated();

    if (service->state() == QLowEnergyService::DiscoveryRequired) {
        connect(service, SIGNAL(stateChanged(QLowEnergyService::ServiceState)), this, SLOT(serviceDetailsDiscovered(QLowEnergyService::ServiceState)));
        service->discoverDetails();
        return;
    }

    //discovery already done
    const QList<QLowEnergyCharacteristic> chars = service->characteristics();
    foreach (const QLowEnergyCharacteristic &ch, chars) {
        CharacteristicInfo *cInfo = new CharacteristicInfo(ch);
        m_characteristics.append(cInfo);
    }
    setUpdate("Bluetooth connected");
    QTimer::singleShot(0, this, SIGNAL(characteristicsUpdated()));
}

void Device::deviceConnected()
{
    connected = true;
    controller->discoverServices();
}

void Device::errorReceived(QLowEnergyController::Error /*error*/)
{
    qWarning() << "Error: " << controller->errorString();
}

void Device::setUpdate(QString message)
{
    emit updateStatus(message);
}

void Device::disconnectFromDevice()
{
     //disabled notifications -> assume disconnect intent
    qDebug() << "Device::disconnectFromDevice()";

    if(transmitService != nullptr) {
        transmitService = nullptr;
        transmitPointer = 0;
    }

    if (controller->state() != QLowEnergyController::UnconnectedState){
        controller->disconnectFromDevice();
        setDeviceVisuallyConnected(false);

        disconnect(controller, SIGNAL(disconnected()), this, SLOT(deviceDisconnected()));
    } else
        deviceDisconnected();
}

void Device::deviceDisconnected()
{
    qWarning() << "Device::deviceDisconnected(): Disconnect from device";
    setDeviceVisuallyConnected(false);
    setUpdate("Disconnected");
    emit disconnected();
    emit qstateChanged();
}

void Device::serviceDetailsDiscovered(QLowEnergyService::ServiceState newState)
{
    if (newState != QLowEnergyService::ServiceDiscovered) {
        if (newState != QLowEnergyService::DiscoveringServices) QMetaObject::invokeMethod(this, "characteristicsUpdated", Qt::QueuedConnection);
        return;
    }

    QLowEnergyService *service = qobject_cast<QLowEnergyService *>(sender());
    if (!service) return;

    const QList<QLowEnergyCharacteristic> chars = service->characteristics();
    foreach (const QLowEnergyCharacteristic &ch, chars) {
        CharacteristicInfo *cInfo = new CharacteristicInfo(ch);
        m_characteristics.append(cInfo);
        for(int i = 0; i < service->characteristics().count(); i++) {
            //qDebug() << service->serviceUuid();
            if(service->serviceUuid().toString().contains("0000ffe0") ) {
                if(transmitService != nullptr) {
                    transmitService = nullptr;
                    transmitPointer = 0;
                }
                transmitService = service;
                transmitPointer = i;
                // Tell user that we are not connected
                setDeviceVisuallyConnected(false);
                // enable notifications, required for two way communication

                const QLowEnergyCharacteristic hrChar = transmitService->characteristics().at(transmitPointer);

                if (!hrChar.isValid()) {
                    break;
                }

                QLowEnergyDescriptor m_notificationDesc = hrChar.descriptor(QBluetoothUuid::ClientCharacteristicConfiguration);
                if (m_notificationDesc.isValid()) {
                    // Enable notifications
                    transmitService->writeDescriptor(m_notificationDesc, QByteArray::fromHex("0100"));
                }

                connect(transmitService, SIGNAL(stateChanged(QLowEnergyService::ServiceState)), this, SLOT(serialStateChanged(QLowEnergyService::ServiceState)));
                connect(transmitService, SIGNAL(characteristicChanged(QLowEnergyCharacteristic, QByteArray)), this, SLOT(serialReadValue(QLowEnergyCharacteristic,QByteArray)));
                connect(transmitService, SIGNAL(descriptorWritten(QLowEnergyDescriptor, QByteArray)), this, SLOT(serialDescriptorWrite(QLowEnergyDescriptor,QByteArray)));
                setDeviceVisuallyConnected(true);
            }
        }
    }
    emit characteristicsUpdated();
}

void Device::deviceScanError(QBluetoothDeviceDiscoveryAgent::Error error)
{
    Q_UNUSED(error)
    m_deviceScanState = false;
    emit devicesUpdated();
    emit qstateChanged();
}

bool Device::state()
{
    return m_deviceScanState;
}

bool Device::hasControllerError() const
{
    if (controller && controller->error() != QLowEnergyController::NoError)
        return true;
    return false;
}

bool Device::isRandomAddress() const
{
    return randomAddress;
}

void Device::setRandomAddress(bool newValue)
{
    randomAddress = newValue;
    emit randomAddressChanged();
}

void Device::readData()
{
    if(transmitService == nullptr) {
        qDebug() << "transmitService not found";
        return;
    }

    for(int y = 0; y < transmitService->characteristics().count(); y++) {
        transmitService->readCharacteristic(transmitService->characteristics().at(transmitPointer));
        qDebug() << transmitService->characteristics().at(y).value();
    }
}

void Device::serialStateChanged(QLowEnergyService::ServiceState s)
{
    qDebug() << "Serial state changed";
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

void Device::setDeviceVisuallyConnected(bool state)
{
    if(!deviceVisuallyConnected && state == true) {
        deviceVisuallyConnected = state;
       setUpdate("Bluetooth connected");
       qDebug() << "Device::setDeviceVisuallyConnected(): connected";
    }
}

void Device::serialReadValue(const QLowEnergyCharacteristic &c, const QByteArray &value)
{
    // ignore any other characteristic change -> shouldn't really happen though
    if (c.uuid().toString().contains("0000ffe0"))
        return;
    //const char *data = value.constData();
    QByteArray startByte = "#";
    QByteArray endByte = "*";
    int startIndex = value.indexOf(startByte) + 1;
    int endIndex = value.indexOf(endByte);
    int difference = endIndex - startIndex;

    if(difference > 5 && value.contains(startByte) && value.contains(endByte)) {
        rxMessage.clear();
        QByteArray temp;
        temp = value;
        // find index of start byte and add 1 to remove it
        //qDebug() << "Device::serialReadValue, whole message in bytes before parsing: " << temp;
        int start = temp.indexOf(startByte) + 1;
        int end = temp.indexOf(endByte);
        if((end - start) < 0) return;   // We have bad message
        // Remove start and end byte
        temp = temp.mid( (start), (end - 1 ));
        rxMessage.append(temp);
        //qDebug() << "Device::serialReadValue, whole message in bytes: " << rxMessage;
        emit messageReceived(rxMessage);
    } else if(value.contains(startByte) && !enableMessageReading) {
        rxMessage.clear();
        QByteArray temp;
        temp = value;
        // find index of start byte and add 1 to remove it
        //qDebug() << "Device::serialReadValue, partial message in bytes: " << temp;
        int start = temp.indexOf(startByte) + 1;
        temp.remove(0, start);
        rxMessage.append(temp);
        enableMessageReading = true;
    } else if(enableMessageReading && value.contains(endByte)) {
        enableMessageReading = false;   // Disable message reading
        QByteArray temp;
        temp = value;
        int end = temp.indexOf(endByte);
        temp = temp.mid(0, (end));
        rxMessage.append(temp);
        emit messageReceived(rxMessage);
        setDeviceVisuallyConnected(true);
       // qDebug() << "Device::serialReadValue, Message in bytes: " << rxMessage;
    } else if(enableMessageReading){
        rxMessage.append(value);
    }
}

void Device::serialDescriptorWrite(const QLowEnergyDescriptor &d, const QByteArray &value)
{
    // Disconnect from device
    qDebug() << "Device::serialDescriptorWrite() " << value;
    const QLowEnergyCharacteristic hrChar = transmitService->characteristics().at(transmitPointer);

    if (!hrChar.isValid()) return;
    QLowEnergyDescriptor m_notificationDesc = hrChar.descriptor(QBluetoothUuid::ClientCharacteristicConfiguration);
    if (d.isValid() && d == m_notificationDesc && value == QByteArray("0000")) {
        //disabled notifications -> assume disconnect intent
        controller->disconnectFromDevice();
        if(transmitService != nullptr) {
            delete transmitService;
            transmitPointer = 0;
        }
    }
}

void Device::transmitData(QString cmd)
{
    if(transmitService == nullptr) {
        qDebug() << "transmitService not found";
        return;
    }
    QByteArray msg;
    msg.append('#');    // Start byte
    msg.append(cmd.toLocal8Bit());  //
    msg.append('*');    // End byte
    transmitService->writeCharacteristic(transmitService->characteristics().at(transmitPointer), msg, QLowEnergyService::WriteWithoutResponse);
    qDebug() << "Device::transmitData() " + msg;
}

bool Device::isConnected(){
   return deviceVisuallyConnected;
}
