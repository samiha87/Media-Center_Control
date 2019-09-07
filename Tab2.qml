import QtQuick 2.9
import QtQuick.Controls 2.0
import com.pages.control 1.0

Page {
    width: 600
    height: 400
    ControlHandler {
        id: controlHandler
        onStatusChanged: {
            console.log("QML, Tab2::ControlHandler() Status changed")
            displayButtonImage.source = getDisplaySource()
            musicButtonImage.source = getAudioSource()
            lightsButtonImage.source = getLightsSource()
        }
    }

    Button {
        id: displayButton
        anchors.top: parent.top
        anchors.topMargin: 60
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

    Button {
        id: speakerButton
        anchors.top: displayButton.bottom
        anchors.topMargin: 20
        anchors.left: parent.left
        anchors.leftMargin: (parent.width*0.05)
        width: 120;
        height: 120;
        onClicked: {
            controlHandler.audioClicked();
        }

        background: Image {
            id: musicButtonImage
             source: controlHandler.getAudioImage ? controlHandler.getAudioImage :"qrc:/icons/musicoff.png";
             // Width and height has to be declared. Otherwise when source updated from control handler
             // Width and height triples.
             width: parent.width
             height: parent.height
        }
    }

    Button {
        id: lightsButton
        anchors.top: displayButton.bottom
        anchors.topMargin: 32
        anchors.right: parent.right
        anchors.rightMargin: (parent.width*0.05)
        width: 120;
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
        text: qsTr("Status bar")
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
