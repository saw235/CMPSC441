import QtQuick 2.5
import QtQuick.Window 2.2
import QtQuick.Controls 2.0

Window {

    id: window1
    visible: true
    width: 376
    height: 387
    title: qsTr("Lab 04")

    // signals
    signal okPressed(string msg);

    // slots
    function updateSurprise(text){
        movieTxt.text = text;
    }
    function updateWatched(text){
        watchedTxt.text = text;
    }
    function updateCurrent(text){
        currTxt.text = text;
    }
    function updatePrice(text){
        priceTxt.text = text;
    }

    Rectangle {
        property alias mouseArea: mouseArea

        width: 376
        height: 387

        MouseArea {
            id: mouseArea
            visible: true
            anchors.rightMargin: 0
            anchors.bottomMargin: 0
            anchors.leftMargin: 0
            anchors.topMargin: 0
            anchors.fill: parent

            Text {
                id: movieLabel
                x: 21
                y: 138
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
                y: 135
                width: 186
                height: 20
                text: qsTr("")
                font.pixelSize: 12
            }

            TextEdit {
                id: watchedTxt
                x: 150
                y: 207
                width: 186
                height: 20
                text: qsTr("")
                font.pixelSize: 12
            }

            TextEdit {
                id: currTxt
                x: 150
                y: 243
                width: 186
                height: 20
                font.pixelSize: 12
            }

            TextInput {
                id: input
                x: 21
                y: 56
                width: 229
                height: 20
                text: "Enter Number"
                cursorVisible: false
                inputMask: ""
                passwordCharacter: qsTr("•")
                horizontalAlignment: Text.AlignLeft
                transformOrigin: Item.Center
                font.pixelSize: 12
            }

            Button {
                id: okBtn
                x: 273
                y: 56
                width: 63
                height: 20
                text: qsTr("Ok")
                onClicked: {
                    window1.okPressed(input.text);
                }
            }

            Button {
                id: quitBtn
                x: 273
                y: 322
                width: 63
                height: 21
                text: qsTr("Quit")
                onClicked:{
                    Qt.quit();
                }
            }

            Text {
                id: price
                x: 21
                y: 172
                text: qsTr("Rental Price:")
                font.pixelSize: 12
            }

            TextEdit {
                id: priceTxt
                x: 150
                y: 169
                width: 186
                height: 20
                text: qsTr("")
                font.pixelSize: 12
            }
        }
    }

//    MainForm {
//        anchors.fill: parent
//        mouseArea.onClicked: {
//            console.log(qsTr('Clicked on background. Text: "' + textEdit.text + '"'))
//        }
//    }
}
