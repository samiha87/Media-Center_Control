import QtQuick 2.9
import QtQuick.Controls 2.1

ApplicationWindow {
    property alias dashboardHeight: root.height
    property alias dashboardWidth: root.width

    id: root
    visible: true
    width: 640
    height: 480
    title: qsTr("Tabs")

    SwipeView {
        id: swipeView
        anchors.fill: parent
        currentIndex: tabBar.currentIndex

        Tab1 {
            id: playerTab
        }

        Tab2 {
            id: controlTab
        }
    }
    header: Rectangle {
        id: headerBar
        width: root.width
        height: (root.height * 0.18)
        color: "steelblue"

        Text {
            id:headerTextField
            text: "Media Center"
            color: "white"
            width: root.width
            height: (parent.height * 0.4)
            anchors.horizontalCenter: parent.horizontalCenter
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            anchors.top: parent.top
            font.pixelSize: 25
        }

        TabBar {
            id: tabBar
            currentIndex: swipeView.currentIndex
            width: parent.width;
            height: (parent.height * 0.4)
            anchors.bottom:headerBar.bottom

            background: Rectangle {
                color:"steelblue"
                Rectangle {
                    width: parent.width / swipeView.count;
                    height: 4;
                    color: (tabBar.currentIndex === 0) ? "red" : "steelblue"
                    anchors.bottom: parent.bottom
                }
                Rectangle {
                    width: parent.width / swipeView.count;
                    height: 4;
                    color: (tabBar.currentIndex === 1) ? "red" : "steelblue"
                    anchors.bottom: parent.bottom
                    anchors.right: parent.right
                }
            }

            TabButton {
                text: qsTr("PLAYER")
                contentItem: Text {
                    text: parent.text
                    color: "white"
                    horizontalAlignment: Text.AlignHCenter
                }

                background: Rectangle {
                    color : "steelblue"
                }
            }
            TabButton {
                text: qsTr("CONTROL")
                background: Rectangle {
                    height: parent.height
                    color : "steelblue"
                }

                contentItem: Text {
                    text: parent.text
                    color: "white"
                    horizontalAlignment: Text.AlignHCenter
                }
            }
        }
    }
}
