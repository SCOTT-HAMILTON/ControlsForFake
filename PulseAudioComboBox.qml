import QtQuick.Controls 2.12
import QtQml.Models 2.12
import Qt.labs.platform 1

ComboBox {
	id: comboBox
	property int staticY: 0
	property int staticHeight: 0
	textRole: "description"

	signal itemChanged(var name, var description, var index)

	onCurrentIndexChanged: {
		// if (currentIndex == -1) return
		// let item = listModel.get(currentIndex)
		// itemChanged(item.name, item.description, item.index)
	}

	onPressedChanged: {
		if (pressed){
			updateModel()
		}
	}

	property var setInit: function () {
	}

	property var updateModel
}
