#include <QQmlApplicationEngine>
#include <QAndroidJniObject>
#include <QtAndroid>

// Project includes
#include "core.h"
#include "device.h"

Core::Core(QObject *parent) : QObject(parent)
{
    controlPage = new ControlPageHandler(this);
    playerPage = new PlayerPageHandler(this);
    // Tell controlPage to use BLE
}

void Core::initialize() {
    // Create classes

    // Check that all required permissions are allowed
    checkPermissions();
    // Register classes to QML
    qmlRegisterType<ControlPageHandler>("com.pages.control", 1, 0, "ControlHandler");
    qmlRegisterType<PlayerPageHandler>("com.pages.player", 1, 0, "PlayerHandler");
    // Initialize icon properties
    initIcons();
    // Start locating BLE device
}

void Core::initIcons() {
    // Initialize control page
    // Display properties
    controlPage->setDisplayImage("projectoroff");
    // Music properties
    controlPage->setAudioImage("speakeroff");
    // Light properties
    controlPage->setLightImage("lightsoff");
}

void Core::checkPermissions()
{/*
    auto result = QtAndroid::checkPermission(QString("android.permission.ACCESS_COARSE_LOCATION"));
    if(result == QtAndroid::PermissionResult::Denied){
        QtAndroid::PermissionResultMap resultHash = QtAndroid::requestPermissionsSync(QStringList({"android.permission.ACCESS_COARSE_LOCATION"}));
        if(resultHash["android.permission.ACCESS_COARSE_LOCATION"] == QtAndroid::PermissionResult::Denied) {
            qDebug() << "Core::checkPermissions() Access to coarse location denied";
        }
    }

   result = QtAndroid::checkPermission(QString("android.permission.ACCESS_FINE_LOCATION"));
    if(result == QtAndroid::PermissionResult::Denied){
        QtAndroid::PermissionResultMap resultHash = QtAndroid::requestPermissionsSync(QStringList({"android.permission.ACCESS_FINE_LOCATION"}));
        if(resultHash["android.permission.ACCESS_FINE_LOCATION"] == QtAndroid::PermissionResult::Denied) {
            qDebug() << "Core::checkPermissions() Access to fine location denied";
        }
    }*/
}
