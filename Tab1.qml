import QtQuick 2.9
import QtQuick.Controls 2.0

Page {
    width: 600
    height: 400

    // Text should slide here
    Label {
        text: qsTr("Status bar")
        anchors.bottom: parent.bottom
    }
}
