import QtQuick 2.11
import QtQuick.Controls 2.2
import com.pages.control 1.0

Page {
    width: 600
    height: 400
    ControlHandler {
        id: controlHandler
        onStatusChanged: {
            console.log("QML, Tab2::ControlHandler() Status changed")
            console.log("QML, Tab2::ControlHandler()" + getDisplaySource())
            displayButtonImage.source = getDisplaySource()
            musicButtonImage.source = getAudioSource()
            lightsButtonImage.source = getLightsSource()
        }
        onTextChanged: {
            statusLabel.text= getStatusText()
        }
    }

    Button {
        id: displayButton
        anchors.top: parent.top
        anchors.topMargin: 30
        anchors.horizontalCenter: parent.horizontalCenter
        width: 120
        height: 120
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
        onPressed: {
            controlHandler.volumeDownClicked();
        }
    }

    ArrowButton {
        id: volumeUpButtown
        anchors.bottom: speakerButton.top
        anchors.bottomMargin: 5
        anchors.left: parent.left
        anchors.leftMargin: 45
        onPressed: {
            //controlHandler.volumeUpClicked();
        }
    }

    Button {
        id: speakerButton
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
