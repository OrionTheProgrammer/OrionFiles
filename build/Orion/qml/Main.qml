import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Orion.Backend

ApplicationWindow {
    id: root

    width: 1200
    height: 760
    visible: true
    title: "Orion"

    color: "#090B10"

    property color backgroundColor: "#090B10"
    property color panelColor: "#111620"
    property color cardColor: "#151B26"
    property color hoverColor: "#1D2636"
    property color accentColor: "#7DF9FF"
    property color textColor: "#E6F1FF"
    property color mutedTextColor: "#8A94A6"

    FileSystemModel {
        id: fileSystem

        onErrorOccurred: function(message) {
            errorText.text = message
            errorPopup.open()
        }
    }

    function formatBytes(bytes) {
        if (bytes < 1024) {
            return bytes + " B"
        }

        var kb = bytes / 1024

        if (kb < 1024) {
            return kb.toFixed(1) + " KB"
        }

        var mb = kb / 1024

        if (mb < 1024) {
            return mb.toFixed(1) + " MB"
        }

        var gb = mb / 1024
        return gb.toFixed(1) + " GB"
    }

    Rectangle {
        anchors.fill: parent
        color: backgroundColor

        gradient: Gradient {
            GradientStop { position: 0.0; color: "#101522" }
            GradientStop { position: 0.5; color: "#090B10" }
            GradientStop { position: 1.0; color: "#050609" }
        }

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 14
            spacing: 12

            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 66
                radius: 18
                color: panelColor
                border.color: "#1F2A3D"
                border.width: 1

                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 12
                    spacing: 10

                    Button {
                        text: "←"
                        Layout.preferredWidth: 46
                        Layout.preferredHeight: 38
                        onClicked: fileSystem.goUp()
                    }

                    Button {
                        text: "⟳"
                        Layout.preferredWidth: 46
                        Layout.preferredHeight: 38
                        onClicked: fileSystem.refresh()
                    }

                    TextField {
                        id: pathField

                        Layout.fillWidth: true
                        Layout.preferredHeight: 38

                        text: fileSystem.currentPath
                        color: textColor
                        selectedTextColor: "#000000"
                        selectionColor: accentColor
                        placeholderText: "Ruta actual"

                        onAccepted: fileSystem.setCurrentPath(text)

                        background: Rectangle {
                            radius: 12
                            color: "#0B0F17"
                            border.color: "#26344A"
                        }
                    }

                    Button {
                        text: "+ Carpeta"
                        Layout.preferredHeight: 38
                        onClicked: newFolderPopup.open()
                    }
                }
            }

            RowLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true
                spacing: 12

                Rectangle {
                    Layout.preferredWidth: 230
                    Layout.fillHeight: true
                    radius: 18
                    color: panelColor
                    border.color: "#1F2A3D"
                    border.width: 1

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 14
                        spacing: 10

                        Label {
                            text: "ORION"
                            color: accentColor
                            font.pixelSize: 24
                            font.bold: true
                        }

                        Label {
                            text: "Gestor de archivos"
                            color: mutedTextColor
                            font.pixelSize: 12
                        }

                        Rectangle {
                            Layout.fillWidth: true
                            Layout.preferredHeight: 1
                            color: "#26344A"
                        }

                        Button {
                            text: "Inicio"
                            Layout.fillWidth: true
                            onClicked: fileSystem.setCurrentPath(fileSystem.homePath())
                        }

                        Button {
                            text: "Descargas"
                            Layout.fillWidth: true
                            onClicked: fileSystem.setCurrentPath(fileSystem.downloadsPath())
                        }

                        Button {
                            text: "Escritorio"
                            Layout.fillWidth: true
                            onClicked: fileSystem.setCurrentPath(fileSystem.desktopPath())
                        }

                        Item {
                            Layout.fillHeight: true
                        }

                        Label {
                            text: "v0.1 MVP"
                            color: mutedTextColor
                            font.pixelSize: 12
                        }
                    }
                }

                Rectangle {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    radius: 18
                    color: "#0D1118"
                    border.color: "#1F2A3D"
                    border.width: 1

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 14
                        spacing: 10

                        RowLayout {
                            Layout.fillWidth: true
                            Layout.preferredHeight: 32

                            Label {
                                text: "Contenido"
                                color: textColor
                                font.pixelSize: 18
                                font.bold: true
                            }

                            Item {
                                Layout.fillWidth: true
                            }

                            Label {
                                text: fileSystem.currentPath
                                color: mutedTextColor
                                font.pixelSize: 12
                                elide: Text.ElideLeft
                                Layout.maximumWidth: 520
                            }
                        }

                        ListView {
                            id: fileList

                            Layout.fillWidth: true
                            Layout.fillHeight: true

                            model: fileSystem
                            clip: true
                            spacing: 6

                            delegate: Rectangle {
                                id: row

                                required property int index
                                required property string name
                                required property string path
                                required property bool isDir
                                required property int size
                                required property string lastModified
                                required property string iconName

                                width: fileList.width
                                height: 52
                                radius: 14

                                color: mouseArea.containsMouse ? hoverColor : "transparent"
                                border.color: mouseArea.containsMouse ? "#31415C" : "transparent"
                                border.width: 1

                                RowLayout {
                                    anchors.fill: parent
                                    anchors.leftMargin: 14
                                    anchors.rightMargin: 14
                                    spacing: 12

                                    Label {
                                        text: row.isDir ? "📁" : "📄"
                                        font.pixelSize: 22
                                    }

                                    Label {
                                        text: row.name
                                        color: textColor
                                        font.pixelSize: 14
                                        elide: Text.ElideRight
                                        Layout.fillWidth: true
                                    }

                                    Label {
                                        text: row.isDir ? "Carpeta" : formatBytes(row.size)
                                        color: mutedTextColor
                                        font.pixelSize: 12
                                        Layout.preferredWidth: 100
                                    }

                                    Label {
                                        text: row.lastModified
                                        color: mutedTextColor
                                        font.pixelSize: 12
                                        Layout.preferredWidth: 135
                                    }
                                }

                                MouseArea {
                                    id: mouseArea
                                    anchors.fill: parent
                                    hoverEnabled: true
                                    acceptedButtons: Qt.LeftButton

                                    onDoubleClicked: {
                                        fileSystem.open(row.index)
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    Popup {
        id: newFolderPopup

        modal: true
        focus: true
        width: 360
        height: 180
        anchors.centerIn: parent
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

        background: Rectangle {
            radius: 18
            color: panelColor
            border.color: accentColor
            border.width: 1
        }

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 18
            spacing: 12

            Label {
                text: "Nueva carpeta"
                color: textColor
                font.pixelSize: 18
                font.bold: true
            }

            TextField {
                id: folderNameInput

                Layout.fillWidth: true
                placeholderText: "Nombre de la carpeta"

                background: Rectangle {
                    radius: 12
                    color: "#0B0F17"
                    border.color: "#26344A"
                }
            }

            RowLayout {
                Layout.fillWidth: true

                Item {
                    Layout.fillWidth: true
                }

                Button {
                    text: "Cancelar"
                    onClicked: newFolderPopup.close()
                }

                Button {
                    text: "Crear"

                    onClicked: {
                        if (fileSystem.createFolder(folderNameInput.text)) {
                            folderNameInput.text = ""
                            newFolderPopup.close()
                        }
                    }
                }
            }
        }
    }

    Popup {
        id: errorPopup

        modal: false
        focus: false
        width: 520
        height: 96
        x: parent.width - width - 24
        y: 24

        background: Rectangle {
            radius: 16
            color: "#2A1118"
            border.color: "#FF5370"
            border.width: 1
        }

        Label {
            id: errorText

            anchors.fill: parent
            anchors.margins: 16
            color: "#FFD6DE"
            wrapMode: Text.WordWrap
        }
    }
}
