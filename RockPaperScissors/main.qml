import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls
import "CustomControls" as CC

Window {
    width: 640
    height: 480
    minimumWidth: statistics.width + to_main_menu_pb.width + 90
    minimumHeight: statistics.height + choose_lbl.height + scissors_pb.height + result_label_lbl.height + sign_img.height + result_lbl.height + 200
    visible: true
    title: qsTr("Rock Scissors Paper")
    signal signalQuit
    signal signalSaveOptions(bool option)
    signal updStatisticsReq
    signal updStatisticsReply(W: int, D: int, L: int)
    signal gameRoundResult(string UserPick, string Sign, string CompPick, string Result, string Color)

    onUpdStatisticsReply: (W, D, L) => {
        let W_pc = 100 * W/(W + D + L);
        let D_pc = 100 * D/(W + D + L);
        let L_pc = 100 * L/(W + D + L);
        let Wr = 100 * W/(W + L);
        wdl_stat_lbl.text = "W: "+W+"("+W_pc.toFixed(2)+"%)"+", D: "+D+"("+D_pc.toFixed(2)+"%)"+", L: "+L+"("+L_pc.toFixed(2)+"%)";
        winrate_lbl.text = "Winrate: " + Wr.toFixed(2) + "%";
    }

    onGameRoundResult: (UserPick, Sign, CompPick, Result, Color) => {
        users_pick_img.source = UserPick;
        sign_img.source = Sign;
        computers_pick_img.source = CompPick;
        result_lbl.text = Result;
        result_lbl.color = Color;
        updStatisticsReq();
    }

    //menu
    //------------
    Rectangle {
        id: menu
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        visible: true

        Text {
            id: menu_lbl
            text: qsTr("Menu")
            font.pointSize: 14
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: start_lbl.top
            anchors.bottomMargin: 70
        }

        Text {
            id: start_lbl
            text: qsTr("start")
            font.pointSize: 12
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: options_lbl.top
            anchors.bottomMargin: 10
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    menu.visible = false
                    game.visible = true
                    updStatisticsReq();
                }
            }
        }

        Text {
            id: options_lbl
            text: qsTr("options")
            font.pointSize: 12
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    menu.visible = false
                    options.visible = true
                }
            }
        }

        Text {
            id: quit_lbl
            text: qsTr("quit")
            font.pointSize: 12
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: options_lbl.bottom
            anchors.topMargin: 10

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    signalQuit();
                }
            }
        }
    }

    //options menu
    //------------

    Rectangle {
        id: options
        width: parent.width
        height: parent.height
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        visible: false

        Text {
            id: computers_pick_lbl
            text: qsTr("Computer picks:")
            font.pointSize: 12
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: first_rb.top
            anchors.bottomMargin: 10
        }

        RadioButton {
            id: first_rb
            anchors.bottom: parent.verticalCenter
            anchors.bottomMargin: 5
            anchors.left: computers_pick_lbl.left
            checked: true
            text: qsTr("Computer picks randomly")
        }
        RadioButton {
            id: second_rb
            anchors.top: parent.verticalCenter
            anchors.topMargin: 5
            anchors.left: computers_pick_lbl.left
            text: qsTr("Computer analyses your choices")
        }

        Text {
            id: save_config_lbl
            text: qsTr("Save")
            font.pointSize: 12
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: second_rb.bottom
            anchors.topMargin: 10
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    options.visible = false
                    menu.visible = true
                    signalSaveOptions(second_rb.checked)
                }
            }
        }
    }

    //game menu
    //------------
    Rectangle {
        id: game
        width: parent.width
        height: parent.height
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        visible: false

        Rectangle {
            id: to_main_menu_pb
            color: "lightgray"
            width: button_name_lbl.width + 20
            height: button_name_lbl.height + 20
            anchors.top: parent.top
            anchors.topMargin: parent.height/25
            anchors.left: parent.left
            anchors.leftMargin: 30
            anchors.rightMargin: 15

            Text {
                id: button_name_lbl
                anchors.centerIn: parent
                text: qsTr("Back to main menu")
                font.pointSize: 12
            }

            MouseArea {
                anchors.fill: parent
                onPressed: {
                    parent.color = "gray"
                }
                onReleased: {
                    parent.color = "lightgray"
                }
                onClicked: {
                    game.visible = false
                    menu.visible = true
                }
            }

        }

        Rectangle {
            id: statistics
            anchors.top: parent.top
            anchors.topMargin: parent.height/25
            anchors.right: parent.right
            anchors.rightMargin: 30
            anchors.leftMargin: 15
            width: wdl_stat_lbl.width
            height: wdl_stat_lbl.height + winrate_lbl.height + 30

            Text {
                id: wdl_stat_lbl
                anchors.top: parent.top
                anchors.topMargin: parent.height/25
                font.pointSize: 12
            }

            Text {
                id: winrate_lbl
                anchors.top: wdl_stat_lbl.bottom
                anchors.topMargin: parent.height/25
                font.pointSize: 12
            }
        }

        Text {
            id: choose_lbl
            anchors.top: statistics.bottom
            anchors.topMargin: parent.height/25
            anchors.horizontalCenter: parent.horizontalCenter
            font.pointSize: 14
            text: qsTr("Make your choice:")
        }

        CC.GameButton {
            id: rock_pb
            name: 114
            file: "qrc:/res/rock.jpg"
            anchors.top: choose_lbl.bottom
            anchors.topMargin: parent.height/25
            anchors.right: scissors_pb.left
            anchors.rightMargin: 50
            width: parent.width/4
            height: parent.height/4
            objectName: "RockButton"
        }

        CC.GameButton {
            id: scissors_pb
            name: 115
            file: "qrc:/res/scissors.jpg"
            anchors.top: choose_lbl.bottom
            anchors.topMargin: parent.height/25
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width/4
            height: parent.height/4
            objectName: "ScissorsButton"
        }

        CC.GameButton {
            id: paper_pb
            name: 112
            file: "qrc:/res/paper.jpg"
            anchors.top: choose_lbl.bottom
            anchors.topMargin: parent.height/25
            anchors.left: scissors_pb.right
            anchors.leftMargin: 50
            width: parent.width/4
            height: parent.height/4
            objectName: "PaperButton"
        }

        Text {
            id: result_label_lbl
            anchors.top: scissors_pb.bottom
            anchors.topMargin: parent.height/25
            anchors.horizontalCenter: parent.horizontalCenter
            font.pointSize: 14
            text: qsTr("The result:")
        }

        Image {
            id: sign_img
            anchors.top: result_label_lbl.bottom
            anchors.topMargin: parent.height/25
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width/4
            height: parent.height/4
            source: "qrc:/res/equals.png"
        }

        Image {
            id: users_pick_img
            anchors.top: result_label_lbl.bottom
            anchors.topMargin: parent.height/25
            anchors.right: sign_img.left
            anchors.rightMargin: 50
            width: parent.width/4
            height: parent.height/4
            source: "qrc:/res/question.png"
        }

        Image {
            id: computers_pick_img
            anchors.top: result_label_lbl.bottom
            anchors.topMargin: parent.height/25
            anchors.left: sign_img.right
            anchors.leftMargin: 50
            width: parent.width/4
            height: parent.height/4
            source: "qrc:/res/question.png"
        }

        Text {
            id: result_lbl
            anchors.top: users_pick_img.bottom
            anchors.topMargin: parent.height/25
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottomMargin: parent.height/25
            font.pointSize: 16
            text: qsTr("THE RESULT")
            color: "blue"
        }

    }
}
