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

Window {
	id: root
	visible: true
	width: 900
	minimumWidth: width
	maximumWidth: width
	minimumHeight: height
	maximumHeight: height
	height: 200
	title: qsTr("Controls for Fake")
	color: "#333333"
	Material.theme: Material.Dark
	Material.accent: Material.Pink

	property string sourceName: ""
	property int sourceIndex: -1
	property string sourceOutputName: ""
	property int sourceOutputIndex: -1
	property var sinks: []

	function generateSinksCommandLine() {
		return sinks.join(',')
	}

	function generateCommandLine() {
		let command = "./FakeMicWavPlayer "+wavFileText.text
		command += ' '+generateSinksCommandLine()
		if (sourceOutputProcessBinaryNameComboBox.currentText !== "")
		command += ' '+sourceOutputProcessBinaryNameComboBox.currentText
		console.log(command)
		return command
	}

	property var setSinkChecked: function(name, checked) {
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
	property var setSourceChecked: function(name, index, checked) {
		if (!checked){
			sourcesComboBox.displayText = "Default"
			root.sourceName = ""
			root.sourceIndex = -1
		} else {
			root.sourceName = name
			root.sourceIndex = index
			
			for (let i = 0; i < sourcesModel.count; ++i) {
				if (sourcesModel.get(i).index === root.sourceIndex) {
					sourcesComboBox.displayText = sourcesModel.get(i).description
				}
			}
		}
	}

	FileDialog {
		id: wavFileDialog
		defaultSuffix: "wav"
		nameFilters: ["Ogg data, Vorbis audio (*.ogg)"]
		folder: StandardPaths.writableLocation(StandardPaths.MusicLocation)
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
		text: qsTr("Open Ogg file...")
		width: root.width*0.27
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

	Text {
		id: sourceOutputText
		text: qsTr("Source Output")
		width: root.width*0.27
		x: sourceOutputProcessBinaryNameComboBox.x+sourceOutputProcessBinaryNameComboBox.width/2-width/2
		color: "#FFFFFF"
		Component.onCompleted: {
			height = root.height*0.25
			y = root.height*0.3
		}
	}

	ComboBox {
		id: sourceOutputProcessBinaryNameComboBox
		width: root.width*0.27
		x: root.width*0.033
		displayText: "Default"
		model:  ListModel {
			id: sourceOutputsModel
			ListElement { name: ""; processBinary:""; index: -1; isChecked: false }
		}
		delegate: CheckBox {
			text: processBinary
			enabled: index === -1 ?  false : true
			checked: isChecked
			onCheckedChanged: {
				if (index === -1) {
					return
				}
				sourceOutputProcessBinaryNameComboBox.displayText = processBinary
				if (!checked) {
					root.sourceOutputName = ""
					root.sourceOutputIndex = -1
					sourceOutputProcessBinaryNameComboBox.displayText = "Default"
				} else {
					root.sourceOutputName = name
					root.sourceOutputIndex = index
				}
				for (let i = 0; i < sourceOutputsModel.count; ++i) {
					if (sourceOutputsModel.get(i).index === index) {
						sourceOutputsModel.set(i, { "name":name,
						"processBinary":processBinary,
						"index":index, "isChecked": checked})
						break
					}
				}
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
			if (sourceOutputsCount === 0) {
				sourceOutputsModel.append({ "name": qsTr("No Source Output Available"),
					"index": -1, "isChecked": false})
			}
			for (let i = 0; i < sourceOutputsCount; ++i) {
				let sourceOutput = fakelibQmlInterface.sourceOutputAt(i)
				let isChecked = false
				// if (i == 0 && root.sourceOutputIndex === -1) {
				// 	isChecked = true
				// 	displayText = sourceOutput.sourceProcessBinaryName
				// }
				if (root.sourceOutputIndex === sourceOutput.index) {
					isChecked = true
				}
				sourceOutputsModel.append({"name": sourceOutput.name, 
										   "processBinary": sourceOutput.sourceProcessBinaryName,
										   "index": sourceOutput.index, "isChecked": isChecked})
			}
			currentIndex = 0
		}
		Component.onCompleted: {
			height = root.height*0.25
			y = root.height*0.40
			updateSourceOutputsModel();
			let sourceOutputsCount = fakelibQmlInterface.sourceOutputsCount()
			if (sourceOutputsCount > 0) {
				let sourceOutput = fakelibQmlInterface.sourceOutputAt(0)
				for (let i = 0; i < sourceOutputsModel.count; ++i) {
					if (sourceOutputsModel.get(i).index === sourceOutput.index) {
						root.sourceOutputName = sourceOutput.name
						root.sourceOutputIndex = sourceOutput.index
						sourceOutputProcessBinaryNameComboBox.displayText = sourceOutput.name
						sourceOutputsModel.set(i, { "name":sourceOutput.name,
						"index":sourceOutput.index, "isChecked": true})
						break
					}
				}
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
		text: qsTr("Sources")
		width: root.width*0.27
		x: sourcesComboBox.x
		color: "#FFFFFF"
		Component.onCompleted: {
			height = root.height*0.25
			y = root.height*0.3
		}
	}

	ComboBox {
		id: sourcesComboBox
		width: root.width*0.27
		x: root.width/2-width/2
		displayText: "Default"
		model:  ListModel {
			id: sourcesModel
			ListElement { description: "Default"; name: ""; index: -1; isChecked: false}
		}
		signal sourceCheckedChange(var name, var index, var checked)
		delegate: CheckBox {
			id: control
			text: description
			checked: isChecked
			onCheckedChanged: {
				if (checked) {
					sourcesComboBox.displayText = description
					for (let i = 0; i < sourcesModel.count; ++i) {
						if (sourcesModel.get(i).index === index) {
							sourcesModel.set(i, { "description": description,
							"name":name,
							"index":index,
							"isChecked": true})
							break
						}
					}
				}
				sourcesComboBox.sourceCheckedChange(name, index, checked)
			}
		}
		onPressedChanged: {
			if (pressed){
				updateSourcesModel()
			}
		}
		property var updateSourcesModel: function() {
			fakelibQmlInterface.updateSourcesList()
			let sourcesCount = fakelibQmlInterface.sourceCount()
			sourcesModel.clear()
			for (let i = 0; i < sourcesCount; ++i) {
				let source = fakelibQmlInterface.sourceAt(i)
				let isChecked = source.index === root.sourceIndex
				sourcesModel.append({ "description":source.description,
				"name":source.name,
				"index": source.index,
				"isChecked": isChecked})
			}
			currentIndex = 0
		}
		Component.onCompleted: {
			height = root.height*0.25
			y = root.height*0.4
			sourcesComboBox.sourceCheckedChange.connect(root.setSourceChecked)
			updateSourcesModel();
			let sourcesCount = fakelibQmlInterface.sourceCount()
			if (sourcesCount > 0) {
				let source = fakelibQmlInterface.sourceAt(0)
				for (let i = 0; i < sourcesModel.count; ++i) {
					if (sourcesModel.get(i).index === source.index) {
						root.sourceName = source.name
						sourcesComboBox.displayText = source.description
						sourcesModel.set(i, { "description": source.description,
						"name":source.name,
						"index":source.index,
						"isChecked": true})
						break
					}
				}
			}
		}
	}

	Text {
		id: sinkText
		text: qsTr("Sinks")
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
				updateSinksModel()
			}
		}
		property var updateSinksModel: function() {
			fakelibQmlInterface.updateSinksList()
			let sinksCount = fakelibQmlInterface.sinkCount()
			sinksModel.clear()
			if (root.sinks.length === 0 && sinksCount > 0) {
				sinksComboBox.displayText = fakelibQmlInterface.sinkAt(0).description
				root.sinks.push(fakelibQmlInterface.sinkAt(0).name)
			}
			for (let i = 0; i < sinksCount; ++i) {
				let sink = fakelibQmlInterface.sinkAt(i)
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
		width: root.width*0.27
		x: root.width/2-width/2
		enabled: false
		highlighted: true
		onClicked: {
			commandLineText.text = root.generateCommandLine()
			console.log("command line : "+root.generateCommandLine())
			heightAnim.start()
			console.log("Sinks CMD : "+root.sinks)
			console.log("Source : "+root.sourceName)
			fakelibQmlInterface.playOggToApp(wavFileText.text,
				root.sourceName,
				generateSinksCommandLine(),
				sourceOutputProcessBinaryNameComboBox.currentText)
	}
	Component.onCompleted: {
		height = root.height*0.25
		y = root.height*0.95-height
	}
}
}
