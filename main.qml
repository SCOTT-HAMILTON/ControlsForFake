import QtGraphicalEffects 1
import QtQuick 2.12
import QtQuick.Window 2
import QtQuick.Controls 2
import QtQuick.Controls.Material 2
import QtQuick.Controls.Styles 1
import Qt.labs.platform 1

import org.controlfake.sink 1.0

Window {
    id: root
    visible: true
    width: 600
    minimumWidth: width
    maximumWidth: width
    minimumHeight: height
    maximumHeight: height
    height: 200
    title: qsTr("Controls for Fake")
    color: "#333333"
    Material.theme: Material.Dark
    Material.accent: Material.Pink

    property var sinks: []
    property var sourceOutputs: []

    function generateSinksCommandLine() {
        if (sinks.length>0)
            return sinks.join(',')
        return "default"
    }

    function generateCommandLine() {
        let command = "./FakeMicWavPlayer "+wavFileText.text
        command += ' '+generateSinksCommandLine()
        if (sourceOutputProcessBinaryNameComboBox.currentText !== "Default")
            command += ' '+sourceOutputProcessBinaryNameComboBox.currentText
        console.log(command)
        return command
    }

    property var setSinkChecked: function(name, checked) {
        console.log("Sink changed : ",name,", ",checked)
        let sinkIndex = sinks.indexOf(name)
        if (checked && sinkIndex === -1){
            sinks.push(name)
        } else if (!checked && sinkIndex >= -1) {
            sinks.splice(sinkIndex, 1)
        }
        if (sinks.length === 0) {
            sinksComboBox.displayText = "Default"
        } else {
            for (let i = 0; i < sinksModel.count; ++i) {
                if (sinksModel.get(i).name === sinks[0]) {
                    sinksComboBox.displayText = sinksModel.get(i).description
                }
            }

        }
    }

    property var setSourceOutputChecked: function(name, checked) {
        let sourceIndex = sourceOutputs.indexOf(name)
        if (checked && sourceIndex === -1){
            sourceOutputs.push(name)
        } else if (!checked && sourceIndex >= -1) {
            sourceOutputs.splice(sourceIndex, 1)
        }
        if (sourceOutputs.length === 0) {
            sourceOutputProcessBinaryNameComboBox.displayText = "Default"
        } else {
            for (let i = 0; i < sourceOutputsModel.count; ++i) {
                if (sourceOutputsModel.get(i).name === sourceOutputs[0]) {
                    sourceOutputProcessBinaryNameComboBox.displayText = name
                }
            }
        }
    }



    FileDialog {
        id: wavFileDialog
        defaultSuffix: "wav"
        nameFilters: ["PCM 16 bit Wav audio file (*.wav)"]
        onAccepted: {
            wavFileText.text = file.toString().substring(7)
            console.log("file : ",currentFile)
            wavFileText.visible = true
            openWaveFileButton.slideRight()
            wavFileText.slideRight()

            runButton.enabled = true
        }
    }

    Rectangle {
        color: "#CCCCCC"
        width: root.width*0.5
        border.color: "#000000"
        radius: 10

        Component.onCompleted: {
            x = -width*2
            height = root.height*0.15
            y = root.height*0.1
        }
    TextInput {
        id: wavFileText
        visible: true
        width: parent.width*0.9
        text: "lol"
        color: "#0000FF"
        font.pointSize: 13
        clip: true
        anchors.centerIn: parent


        property var slideRight: function() {
            wavFileTextDelayTimer.start()
        }

        onTextChanged: {
//            cursorPosition = 0
        }

        Timer {
            id: wavFileTextDelayTimer
            interval: 100
            repeat: false
            running: false
            triggeredOnStart:  false
            onTriggered: {
                wavFileTextSlideRightAnim.start()
            }
        }

        PropertyAnimation {
            id: wavFileTextSlideRightAnim
            target: wavFileText.parent
            property: "x"
            from: wavFileText.parent.x
            to: root.width*0.02
            easing.type:  Easing.OutCubic
            duration: 1000
        }
    }
    }

    Button {
        id: openWaveFileButton
        text: qsTr("Open wav file...")
        width: root.width*0.4
        onClicked: {
            wavFileDialog.open()
        }
        property var slideRight: function() {
            openWaveFileButtonSlideRightAnim.start()
        }

        PropertyAnimation {
            id: openWaveFileButtonSlideRightAnim
            target: openWaveFileButton
            property: "x"
            from: openWaveFileButton.x
            to: root.width*0.952-openWaveFileButton.width
            easing.type:  Easing.OutCubic
            duration: 1000
        }

        Component.onCompleted: {
            x = root.width*0.5-width/2
            height = root.height*0.25
            y = root.height*0.05
        }
    }

    ComboBox {
        id: sourceOutputProcessBinaryNameComboBox
        width: root.width*0.4
        x: root.width*0.05
        displayText: "Default"

        model:  ListModel {
            id: sourceOutputsModel
            ListElement { name: ""; index: -1; isChecked: false }
        }
        signal binaryNameCheckedChange(var name, var checked)
        delegate: CheckBox {
            text: name
            checked: isChecked

            onCheckedChanged: {
                if (checked) {
                    sourceOutputProcessBinaryNameComboBox.displayText = name
                    for (let i = 0; i < sourceOutputsModel.count; ++i) {
                        if (sourceOutputsModel.get(i).index === index) {
                            sourceOutputsModel.set(i, { "name":name,
                                                        "index":index,
                                                        "isChecked": true})
                            break
                        }
                    }
                }

                sourceOutputProcessBinaryNameComboBox.binaryNameCheckedChange(name, checked)
            }
        }

        onPressedChanged: {
            if (pressed){
                updateSourceOutputsModel()
            }
        }


        property var updateSourceOutputsModel: function() {
            fakelibQmlInterface.updateSourceOuputsList()
            let sourceOutputsCount = fakelibQmlInterface.sourceOutputsCount()
            sourceOutputsModel.clear()
            for (let i = 0; i < sourceOutputsCount; ++i) {
                let sourceOutput = fakelibQmlInterface.sourceOutputAt(i)
                let isChecked = root.sourceOutputs.indexOf(sourceOutput.sourceProcessBinaryName)!==-1
                sourceOutputsModel.append({ "name": sourceOutput.sourceProcessBinaryName,
                                            "index": sourceOutput.index,
                                            "isChecked": isChecked})
            }
            currentIndex = 0
        }

        Component.onCompleted: {
            height = root.height*0.25
            y = root.height*0.4
            sourceOutputProcessBinaryNameComboBox.binaryNameCheckedChange.connect(root.setSourceOutputChecked)
            updateSourceOutputsModel();
        }
    }


    ComboBox {
        id: sinksComboBox
        width: root.width*0.4
        x: root.width*0.95-width
        displayText: "Default"

        model:  ListModel {
            id: sinksModel
            ListElement { description: "Default"; name: ""; index: -1; isChecked: false}
        }
        signal sinkCheckedChange(var name, var checked)
        delegate: CheckBox {
            text: description
            checked: isChecked
            onCheckedChanged: {
                if (checked) {
                    sinksComboBox.displayText = description
                    for (let i = 0; i < sinksModel.count; ++i) {
                        if (sinksModel.get(i).index === index) {
                            sinksModel.set(i, { "description": description,
                                                "name":name,
                                                "index":index,
                                                "isChecked": true})
                            break
                        }
                    }
                }

                sinksComboBox.sinkCheckedChange(name, checked)
            }
        }

        onPressedChanged: {
            if (pressed){
                console.log("pressed")
                updateSinksModel()
            }
        }


        property var updateSinksModel: function() {
            fakelibQmlInterface.updateSinksList()
            let sinksCount = fakelibQmlInterface.sinkCount()
            sinksModel.clear()
            let first = ""
            for (let i = 0; i < sinksCount; ++i) {
                let sink = fakelibQmlInterface.sinkAt(i)
                if (i === 0) first = sink.description
                let isChecked = root.sinks.indexOf(sink.name)!==-1
                sinksModel.append({ "description":sink.description,
                                    "name":sink.name,
                                    "index": sink.index,
                                    "isChecked": isChecked})
            }
            currentIndex = 0
        }

        Component.onCompleted: {
            height = root.height*0.25
            y = root.height*0.4
            sinksComboBox.sinkCheckedChange.connect(root.setSinkChecked)

            updateSinksModel();
        }
    }

    TextInput {
        id: commandLineText
        font.pointSize: 10
        color: "#FFFFFF"
        visible: false
        clip: true
        x: root.width/2-commandLineText.width/2
        verticalAlignment: Qt.AlignVCenter
        width: root.width*0.8

        onTextChanged: copyCommandButton.enabled = true

        Component.onCompleted: {
        }
    }

    Button {
        id: copyCommandButton
        visible: false
        width: root.width*0.045
        height: root.height*0.15
        x: root.width*0.97-width
        y: root.height*0.97-height
        Material.accent: Material.DeepPurple
        highlighted: true
        Image {
            id: copyIcon
            fillMode: Image.PreserveAspectFit
            anchors.centerIn: parent
            sourceSize.height: copyCommandButton.background.height - 6
            height: sourceSize.height
            source: "qrc:/icons/copy.svg"
        }
        ColorOverlay {
            anchors.fill: copyIcon
            source: copyIcon
            // In dark styles background is dark, this makes the image white
            color:"#ffffffff"
        }

        onClicked: enabled = false
    }

    PropertyAnimation {
        id: heightAnim
        target: root
        property: "height"
        from: root.height
        to: 250
        easing.type:  Easing.OutCubic
        duration: 1000
        onStarted: {
            root.maximumHeight = to
        }

        onFinished: {
            commandLineText.y = root.height*0.93-commandLineText.height
            commandLineText.visible = true
            copyCommandButton.visible = true
        }
    }

    Button {
        id: runButton
        text: qsTr("Play")
        width: root.width*0.4
        x: root.width/2-width/2
        enabled: false
        highlighted: true
        onClicked: {
            commandLineText.text = root.generateCommandLine()
            console.log("command line : "+root.generateCommandLine())
            heightAnim.start()
            fakelibQmlInterface.fakePlay(wavFileText.text,
                                         generateSinksCommandLine(),
                                         (sourceOutputProcessBinaryNameComboBox.currentText!==""?
                                            sourceOutputProcessBinaryNameComboBox.currentText:""
                                            ))
        }
        Component.onCompleted: {
            height = root.height*0.25
            y = root.height*0.95-height
        }
    }
}
