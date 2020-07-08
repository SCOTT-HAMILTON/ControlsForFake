import QtQuick.Controls 2.12
import QtQml.Models 2.12
import Qt.labs.platform 1

ComboBox {
	id: comboBox
	property int staticY: 0
	property int staticHeight: 0
	displayText: "Default"

	signal checkedChanged(var name, var description, var index, var checked)

	model:  ListModel {
		id: listModel
		ListElement { name: ""; description:""; index: -1; isChecked: false }
	}
	delegate: CheckBox {
		text: description
		enabled: index === -1 ?  false : true
		checked: isChecked
		onCheckedChanged: {
			if (index === -1) {
				return
			}
			comboBox.displayText = description
			if (!checked) {
				comboBox.displayText = qsTr("Default")
			}
			for (let i = 0; i < model.count; ++i) {
				if (model.get(i).index === index) {
					model.set(i, { "name":name,
					"description":description,
					"index":index, "isChecked": checked})
					break
				}
			}
			comboBox.checkedChanged(name, description, index, checked)
		}
	}
	onPressedChanged: {
		if (pressed){
			updateModel()
		}
	}

	property var setInit: function () {
		updateModel()
		if (model.count > 0) {
			let obj = model.get(0)
			model.set(0, { "name":obj.name,
				"description":obj.description,
				"index":obj.index, "isChecked": true})
			displayText = obj.description
			comboBox.checkedChanged(obj.name, obj.description, obj.index, true)
		}
	}

	property var updateModel
		// let sourceOutputsCount = fakelibQmlInterface.sourceOutputsCount()
		// if (sourceOutputsCount > 0) {
		// 	let sourceOutput = fakelibQmlInterface.sourceOutputAt(0)
		// 	for (let i = 0; i < listModel.count; ++i) {
		// 		if (listModel.get(i).index === sourceOutput.index) {
		// 			root.sourceOutputName = sourceOutput.name
		// 			root.sourceOutputIndex = sourceOutput.index
		// 			comboBox.displayText = sourceOutput.name
		// 			listModel.set(i, { "name":sourceOutput.name,
		// 			"index":sourceOutput.index, "isChecked": true})
		// 			break
		// 		}
		// 	}
		// }
}
