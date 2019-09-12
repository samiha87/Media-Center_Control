import QtQuick 2.11
import QtQuick.Controls 2.2
import com.pages.control 1.0

Page {
    id: controlPage
    width: 600
    height: 400
    ControlHandler {
        id: controlHandler
        onActivate:{
            displayButton.enabled = getBleConnected()
            speakerButton.enabled = getBleConnected()
            lightsButton.enabled = getBleConnected()
            volumeDownButton.enabled = getBleConnected()
            volumeUpButton.enabled = getBleConnected()

        }

        onStatusChanged: {
            console.log("QML, Tab2::ControlHandler() Status changed")
            console.log("QML, Tab2::ControlHandler()" + getDisplaySource())
            // Main control icons
            displayButtonImage.source = getDisplaySource()
            musicButtonImage.source = getAudioSource()
            lightsButtonImage.source = getLightsSource()
            // BLE connection related
            bleScan.playing = !getBleConnected()
            bleScan.visible= !getBleConnected()
            // Audio related
            volumeDownButton.visible = getVolumeActive()
            volumeUpButton.visible = getVolumeActive()
            // Activate
        }
        onTextChanged: {
            statusLabel.text= getStatusText()
        }
    }

    AnimatedImage {
        id: bleScan
        source: "qrc:/icons/scan.gif"
        width: 0.3 * controlPage.width
        height: 0.3 * controlPage.height
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        playing: !controlHandler.getBleConnected()
    }


    Button {
        id: displayButton
        anchors.top: parent.top
        anchors.topMargin: 30
        anchors.horizontalCenter: parent.horizontalCenter
        width: 120
        height: 120
        enabled: controlHandler.getBleConnected()
        onClicked: {
            controlHandler.displayClicked();
        }
        background: Image {
            id: displayButtonImage
            source: controlHandler.getDisplaySource() ? controlHandler.getDisplaySource() :"qrc:/icons/projectoroff.png";
            fillMode: Image.Stretch
            // Width and height has to be declared. Otherwise when source updated from control handler
            // Width and height triples.
            width: parent.width
            height: parent.height
        }
    }

    // Volume button with animation
    ArrowButton {
        id: volumeDownButton
        anchors.top: speakerButton.bottom
        anchors.topMargin: 5
        anchors.left: parent.left
        anchors.leftMargin: 45
        rotation: 180
        enabled: controlHandler.getBleConnected()
        visible: controlHandler.getVolumeActive()   // If audio power is on, show button
        onPressed: {
            controlHandler.volumeDownClicked();
        }
    }

    ArrowButton {
        id: volumeUpButton
        enabled: controlHandler.getBleConnected()
        anchors.bottom: speakerButton.top
        anchors.bottomMargin: 5
        anchors.left: parent.left
        anchors.leftMargin: 45
        visible: controlHandler.getVolumeActive()   // If audio power is on, show button
        onPressed: {
            controlHandler.volumeUpClicked();
        }
    }

    Button {
        id: speakerButton
        enabled: controlHandler.getBleConnected()
        anchors.top: displayButton.bottom
        anchors.topMargin: 50
        anchors.left: parent.left
        anchors.leftMargin: 20
        width: 100;
        height: 100;
        onClicked: {
            controlHandler.audioClicked();
        }

        background: Image {
            id: musicButtonImage
             source: controlHandler.getAudioImage ? controlHandler.getAudioImage :"qrc:/icons/speakeroff.png";
             // Width and height has to be declared. Otherwise when source updated from control handler
             // Width and height triples.
             width: parent.width
             height: parent.height
        }
    }

    Button {
        id: lightsButton
        enabled: controlHandler.getBleConnected()
        anchors.top: displayButton.bottom
        anchors.topMargin: 44
        anchors.right: parent.right
        anchors.rightMargin: 30
        width: 100;
        height: 100;    // TODO change icon
        onClicked: {
            controlHandler.lightsClicked();
        }
        background: Image {
            id: lightsButtonImage
            source: controlHandler.getLightImage ? controlHandler.getLightImage :"qrc:/icons/lightbulboff.png";
            // Width and height has to be declared. Otherwise when source updated from control handler
            // Width and height triples.
            width: parent.width
            height: parent.height
        }
    }

    // Text should slide here
    Label {
        id: statusLabel
        text: controlHandler.getStatusText()
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        anchors.right: parent.right
        width: parent.width
        height: parent*0.2
        font.pixelSize: 25
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignTop
    }
}
