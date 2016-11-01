import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")
    property int fsz: 14

    SwipeView {
        id: swipeView
        anchors.fill: parent
        currentIndex: tabBar.currentIndex

        Page {
            Label {
                id: pageLabel
                text: qsTr("First page")
                anchors.top: parent.top
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.topMargin: 30
            }

            ColumnLayout {
                id: positionLayout
                anchors.centerIn: parent
                RowLayout {
                    anchors.left: parent.left; anchors.right: parent.right
                    Label { text: qsTr("Heading:"); font.pointSize: fsz }
                    Label { text: positionn.heading; font.pointSize: fsz }
                }
                RowLayout {
                    anchors.left: parent.left; anchors.right: parent.right
                    Label { text: qsTr("Elevation:"); font.pointSize: fsz }
                    Label { text: positionn.elevation; font.pointSize: fsz }
                }
                RowLayout {
                    anchors.left: parent.left; anchors.right: parent.right
                    Label { text: qsTr("Lat:"); font.pointSize: fsz }
                    Label { text: positionn.latitude; font.pointSize: fsz }
                }
                RowLayout {
                    anchors.left: parent.left; anchors.right: parent.right
                    Label { text: qsTr("Long:"); font.pointSize: fsz }
                    Label { text: positionn.longitude; font.pointSize: fsz }
                }
                RowLayout {
                    anchors.left: parent.left; anchors.right: parent.right
                    Label { text: qsTr("Height Above Ellipsoid:"); font.pointSize: fsz }
                    Label { text: positionn.heightAboveEllipsoid; font.pointSize: fsz }
                }
                RowLayout {
                    anchors.left: parent.left; anchors.right: parent.right
                    Label { text: qsTr("Roll:"); font.pointSize: fsz }
                    Label { text: positionn.roll; font.pointSize: fsz }
                }
            }

            RowLayout {
                id: buttonRow
                anchors.top: positionLayout.bottom
                anchors.horizontalCenter: parent.horizontalCenter

                Button {
                    id: button
                    text: qsTr("Press Me")
                    onClicked: {
                        console.log("Button clicked.");
                    }
                }
            }
        }

        Page1 {
        }
    }

    footer: TabBar {
        id: tabBar
        currentIndex: swipeView.currentIndex
        TabButton {
            text: qsTr("First")
        }
        TabButton {
            text: qsTr("Second")
        }
    }
}
