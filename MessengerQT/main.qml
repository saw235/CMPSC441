import QtQuick 2.5
import QtQuick.Window 2.2
import QtQuick.Controls 2.0

Window {
    id: clientWindow
    visible: true
    width: 640
    height: 480
    title: qsTr("Messenger Client")

    // Signals
    signal connectToServer(string ip, int port);     // connect to server
    signal disconnect();                                // disconnect to server
    signal sendMsg(string msg);                         // send message

    // Slots
    function incomingMsg(msg){                   // incoming message
        chatBox.append(msg);
    }

    function error(e){                          // error messges (connecting to server)
        errorField.text = e;

    }

    Rectangle {
        property alias mouseArea: mouseArea
        //property alias textEdit: textEdit
        x: 0
        y: 0

        width: 640
        height: 480

        MouseArea {
            id: mouseArea
            anchors.rightMargin: 0
            anchors.bottomMargin: 0
            anchors.leftMargin: 0
            anchors.topMargin: 0
            anchors.fill: parent

            Label {
                id: labelIP
                x: 23
                y: 16
                text: qsTr("Server IP:")
                font.pointSize: 11
            }

            Label {
                id: labelPort
                x: 23
                y: 55
                text: qsTr("Server Port:")
                font.pointSize: 11
            }

            TextField {
                id: ipInput
                x: 163
                y: 13
                width: 288
                height: 25
                placeholderText: "Server IP Number"
            }

            TextField {
                id: portInput
                x: 163
                y: 51
                width: 288
                height: 26
                placeholderText: "Server Port Number"
            }

            TextField {
                id: errorField
                x: 23
                y: 90
                width: 595
                height: 29
                text: "Disconnected From Server"
                font.bold: false
                enabled: true
                horizontalAlignment: Text.AlignHCenter
                readOnly: true
            }

            Button {
                id: connectBtn
                x: 518
                y: 37
                text: qsTr("Connect")
                onClicked: {
                    if(connectBtn.text == "Connect"){
                        clientWindow.connectToServer(ipInput.text, portInput.text);
                        connectBtn.text = "Disconnect";
                    }
                    else if(connectBtn.text == "Disconnect"){
                        clientWindow.disconnect();
                        connectBtn.text = "Connect";
                    }

                }
            }

            Button {
                id: quitBtn
                x: 23
                y: 424
                text: qsTr("Quit")
                onClicked:{

                    clientWindow.disconnect();
                    Qt.quit();
                }
            }

            Pane {
                id: pane1
                x: 23
                y: 125
                width: 595
                height: 293

                Button {
                    id: sendBtn
                    x: 483
                    y: 231
                    text: qsTr("Send")
                    onClicked: {
                        clientWindow.sendMsg(messageInput.text);
                        messageInput.text = "";
                    }
                }

                TextField {
                    id: messageInput
                    x: -12
                    y: 231
                    width: 428
                    height: 40
                    //text: qsTr("Text Field")
                    placeholderText: "Message to be sent"
                }

                Flickable {
                    id: flickable
                    anchors.bottomMargin: 44
                    anchors.fill: parent
                    flickableDirection: Flickable.VerticalFlick
                    TextArea.flickable: TextArea {
                        id: chatBox
                        x: -12
                        y: 0
                        width: 595
                        height: 217
                        text: qsTr("Text Area")
                        wrapMode: TextArea.Wrap
                        //readOnly: true
                    }
                    ScrollBar.vertical: ScrollBar {}
                }
            }
        }
    }
}
