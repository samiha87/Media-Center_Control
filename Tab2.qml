import QtQuick 2.9
import QtQuick.Controls 2.0
import com.pages.control 1.0

Page {
    width: 600
    height: 400

    Button {
       id: displayButton
       anchors.top: parent.top
       anchors.topMargin: 60
       anchors.horizontalCenter: parent.horizontalCenter
       width: 120
       height: 120
       onClicked: {

       }
       background: Image {
            source: ControlHandler.imageSource ? "qrc:/icons/projectoroff.png" : ControlHandler.imageSource;
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

        }

        background: Image {
             source: "qrc:/icons/musicoff.png"
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

        }
        background: Image {
             source: "qrc:/icons/lightbulboff.png"
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
