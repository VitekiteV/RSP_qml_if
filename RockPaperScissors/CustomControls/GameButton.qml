import QtQuick 2.15

Rectangle {
    signal buttonClicked(pick: int)
    property int name: 114
    property alias file: pic.source
    id: game_button
    width: 120
    height: 90

    Image {
        id: pic
        height: parent.height
        width: parent.width
        source: "file"
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            buttonClicked(parent.name);
        }
    }
}
