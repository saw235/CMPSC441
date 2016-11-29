import QtQuick 2.5
import QtQuick.Controls 2.0

Rectangle {
    property alias mouseArea: mouseArea

    width: 360
    height: 360

    MouseArea {
        id: mouseArea
        anchors.rightMargin: 0
        anchors.bottomMargin: 0
        anchors.leftMargin: 0
        anchors.topMargin: 0
        anchors.fill: parent

        Text {
            id: movieLabel
            x: 21
            y: 168
            width: 91
            height: 14
            text: qsTr("Surprise Movie:")
            font.pixelSize: 12
        }

        Text {
            id: watchedLabel
            x: 21
            y: 210
            text: qsTr("Last Time Watched:")
            font.pixelSize: 12
        }

        Text {
            id: currTimeLabel
            x: 21
            y: 246
            text: qsTr("Current Time:")
            font.pixelSize: 12
        }

        TextEdit {
            id: movieTxt
            x: 150
            y: 165
            width: 186
            height: 20
            text: qsTr("Text Edit")
            font.pixelSize: 12
        }

        TextEdit {
            id: watchedTxt
            x: 150
            y: 207
            width: 186
            height: 20
            text: qsTr("Text Edit")
            font.pixelSize: 12
        }

        TextEdit {
            id: currTxt
            x: 150
            y: 243
            width: 186
            height: 20
            text: qsTr("Text Edit")
            font.pixelSize: 12
        }

        TextInput {
            id: input
            x: 21
            y: 56
            width: 229
            height: 20
            text: qsTr("Text Input")
            font.pixelSize: 12
        }

        Button {
            id: okBtn
            x: 273
            y: 56
            width: 63
            height: 20
            text: qsTr("Ok")
        }

        Button {
            id: quitBtn
            x: 273
            y: 322
            width: 63
            height: 21
            text: qsTr("Quit")
        }
    }
}
