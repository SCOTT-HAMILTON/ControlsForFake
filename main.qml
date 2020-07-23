import QtGraphicalEffects 1
import QtQuick 2.12
import QtQuick.Window 2
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2
import QtQuick.Controls.Styles 1
import Qt.labs.platform 1

import org.controlfake.sink 1.0
import org.controlfake.source 1.0
import org.controlfake.sourceOutput 1.0
import org.controlfake.sinkInput 1.0

Window {
    id: root
    visible: true
    width: 900
    height: 200
    minimumWidth: width
    maximumWidth: width
    minimumHeight: height
    maximumHeight: height
    title: qsTr("Controls for Fake")
    color: "#333333"
    Material.theme: Material.Dark
    Material.accent: Material.Pink

    property string sinkInputName: ""
    property int sinkInputIndex: -1
	property var sinks: [ ]
	property string sinksDummyUpdate: "" 
	property string sinkInputsDummyUpdate: "" 

	function generateSinksCommandLine() {
		return sinks.join(',');
	}

	function onSinksChanged() {
		sinks.splice(0, sinks.length)
		root.sinksDummyUpdateChanged()
	}
	function onSourcesChanged() {
		if (fakelibQmlInterface.sources.count > 0)
			sourcesComboBox.currentIndex = 0
	}
	function onSourceOutputsChanged() {
		if (fakelibQmlInterface.sourceOutputs.count > 0)
		sourceOutputProcessBinaryNameComboBox.currentIndex = 0
	}
	function onSinkInputsChanged() {
		if (fakelibQmlInterface.sinkInputs.count > 0)
			sinkInputComboBox.currentIndex = 0
		root.sinkInputsDummyUpdateChanged()
	}

    FileDialog {
        id: audioFileDialog
        defaultSuffix: "ogg"
        nameFilters: ["Ogg data, Vorbis audio (*.ogg)"]
		folder: StandardPaths.writableLocation(StandardPaths.MusicLocation)
		file: ""
		fileMode: FileDialog.OpenFile
		onAccepted: {
            audioFileText.text = file.toString().substring(7)
            audioFileText.visible = true
            startStreamButton.slideRight()
            audioFileText.slideRight()
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
            id: audioFileText
            visible: true
            width: parent.width*0.9
            text: ""
            color: "#0000FF"
            font.pointSize: 13
            clip: true
            anchors.centerIn: parent
            property var slideRight: function() {
                audioFileTextDelayTimer.start()
            }
            Timer {
                id: audioFileTextDelayTimer
                interval: 100
                repeat: false
                running: false
                triggeredOnStart:  false
                onTriggered: {
                    audioFileTextSlideRightAnim.start()
                }
            }
            PropertyAnimation {
                id: audioFileTextSlideRightAnim
                target: audioFileText.parent
                property: "x"
                from: audioFileText.parent.x
                to: root.width*0.02
                easing.type:  Easing.OutCubic
                duration: 1000
			}
        }
	}

    Button {
        id: startStreamButton
        width: root.width*0.27
		onClicked: {
			switch (state) {
				case "AudioFile": audioFileDialog.open()
			}
        }
        property var slideRight: function() {
            startStreamButtonSlideRightAnim.start()
        }
        states: [
            State {
                name: "AudioFile"
                PropertyChanges {
                    target: startStreamButton
					text: qsTr("Open OGG File...")
					visible: true
				}
            },
            State {
                name: "Application"
                PropertyChanges {
                    target: startStreamButton
					visible: false
                }
            }
		]
        PropertyAnimation {
            id: startStreamButtonSlideRightAnim
            target: startStreamButton
            property: "x"
            from: startStreamButton.x
            to: root.width*0.9-startStreamButton.width
            easing.type:  Easing.OutCubic
            duration: 1000
        }
        Component.onCompleted: {
            x = root.width*0.5-width/2
            height = root.height*0.25
            y = root.height*0.05
			state = "AudioFile"
        }
    }
	Menu {
		id: audioStreamSourceMenu
		MenuItem { 
			text: qsTr("Audio OGG File")
		    onTriggered: {
				startStreamButton.state = "AudioFile"
				sinkInputComboBox.visible = false
			}
		}
		MenuItem {
			text: qsTr("Application")
		    onTriggered: {
				startStreamButton.state = "Application"
				sinkInputComboBox.visible = true
			}
		}
	}
	SinkInputComboBox {
        id: sinkInputComboBox
        width: startStreamButton.width
        height: startStreamButton.height
        x: startStreamButton.x
		y: startStreamButton.y
		visible: false
    }
    Button {
        id: switchAudioStream
        text: qsTr("...")
        width: root.width*0.05
        x: startStreamButton.x+startStreamButton.width+root.width*0.01
        onClicked: {
            audioStreamSourceMenu.open()
        }
        Component.onCompleted: {
            height = root.height*0.25
            y = root.height*0.05
        }
    }

    Text {
        id: sourceOutputText
        text: qsTr("Recording Applications")
        width: root.width*0.27
        x: sourceOutputProcessBinaryNameComboBox.x+sourceOutputProcessBinaryNameComboBox.width/2-width/2
        color: "#FFFFFF"
        Component.onCompleted: {
            height = root.height*0.25
            y = root.height*0.3
        }
    }

	SourceOutputComboBox {
        id: sourceOutputProcessBinaryNameComboBox
        width: root.width*0.27
		x: root.width*0.033
		Component.onCompleted: {
			y = root.height*0.40
			height = root.height*0.25
		}
		Connections {
			target: fakelibQmlInterface
			function onSourceOutputsChanged() {
				console.log("Source Ouput changed")
			}
		}
    }

    Slider {
        id: sourceOutputVolumeSlider
        width: sourceOutputProcessBinaryNameComboBox.width
        x: sourceOutputProcessBinaryNameComboBox.x
        value: 100
        to: 200
        onValueChanged: {
            fakelibQmlInterface.set_source_volume(value)
        }
        Component.onCompleted: {
            height = root.height*0.25
            y = root.height*0.7
        }
    }

    Text {
        id: sourceText
        text: qsTr("Input Devices")
        width: root.width*0.27
        x: sourcesComboBox.x
        color: "#FFFFFF"
        Component.onCompleted: {
            height = root.height*0.25
            y = root.height*0.3
        }
    }

    SourceComboBox {
        id: sourcesComboBox
        width: root.width*0.27
        x: root.width/2-width/2
        Component.onCompleted: {
            height = root.height*0.25
            y = root.height*0.4
        }
    }

    Text {
        id: sinkText
        text: qsTr("Output Devices")
        width: root.width*0.27
        x: sinksComboBox.x+sinksComboBox.width/2-width/2
        color: "#FFFFFF"
        Component.onCompleted: {
            height = root.height*0.25
            y = root.height*0.3
        }
    }

    ComboBox {
        id: sinksComboBox
        width: root.width*0.27
        x: root.width*0.97-width
		model: fakelibQmlInterface.sinks
		displayText: root.sinks.length+qsTr(" sinks selected")+root.sinksDummyUpdate
        delegate: CheckBox {
            text: description
            checked: root.sinks.includes(name)
			onCheckedChanged: {
				let exists = root.sinks.includes(name)
				if (checked && !exists) {
					root.sinks.push(name)
				} else if (!checked && exists) {
					let sinkIndex = root.sinks.indexOf(name)
					root.sinks.splice(sinkIndex, 1)
				}
				if (root.sinks.length == 1) 
					sinksComboBox.displayText = root.sinks.length+qsTr(" sink selected")
				else
					sinksComboBox.displayText = root.sinks.length+qsTr(" sinks selected")
            }
		}

        Component.onCompleted: {
            height = root.height*0.25
			y = root.height*0.4
			currentIndex = 0
        }
    }

    Slider {
        id: sinksVolumeSlider
        width: sinksComboBox.width
        x: sinksComboBox.x
        value: 100
        to: 200
        onValueChanged: {
            fakelibQmlInterface.set_user_volume(value)
        }
        Component.onCompleted: {
            height = root.height*0.25
            y = root.height*0.7
        }
    }

    Button {
        id: runButton
		width: root.width*0.27
        x: root.width/2-width/2
		enabled:  
		(startStreamButton.state === "AudioFile" && audioFileText.text !== "") ||
		(startStreamButton.state === "Application"+root.sinkInputsDummyUpdate && 
			sinkInputComboBox.currentIndex >= 0 &&
			fakelibQmlInterface.sinkInputs.get(sinkInputComboBox.currentIndex).pa_index !== -1)
        highlighted: true
		onClicked: {
			if (state == "Stop") {
				fakelibQmlInterface.clean();
				state = "Play"
			}
			else if (state == "Play") {
				console.log(sourcesComboBox.currentIndex)
				console.log(sinkInputComboBox.currentIndex)
				console.log(fakelibQmlInterface.sources.get(sourcesComboBox.currentIndex).name)
				console.log(fakelibQmlInterface.sinkInputs.get(sinkInputComboBox.currentIndex).processBinaryName)
				switch (startStreamButton.state) {
					case "AudioFile": {
						fakelibQmlInterface.playOggToApp(audioFileText.text,
							fakelibQmlInterface.sources.get(sourcesComboBox.currentIndex).name,
							generateSinksCommandLine(),
							sourceOutputProcessBinaryNameComboBox.currentText)
						break
					}
					case "Application": {
						fakelibQmlInterface.sendAppSoundToApp(
							fakelibQmlInterface.sinkInputs.get(sinkInputComboBox.currentIndex).processBinaryName,
							fakelibQmlInterface.sources.get(sourcesComboBox.currentIndex).name,
							generateSinksCommandLine(),
							sourceOutputProcessBinaryNameComboBox.currentText)
						state = "Stop"
						break
					}
				}
			}
		}
		states: [
			State {
				name: "Play"
				PropertyChanges {
					target: runButton
					text: qsTr("Play")
				}
			},
			State {
				name: "Stop"
				PropertyChanges {
					target: runButton
					text: qsTr("Stop")
				}
			}
		]
		Connections {
			target: fakelibQmlInterface
			onRunningChanged: {
				console.log("running changed to "+fakelibQmlInterface.running)
			}
		}
			
		state: "Play"
		Component.onCompleted: {
			height = root.height*0.25
			y = root.height*0.95-height
		}
	}
}
