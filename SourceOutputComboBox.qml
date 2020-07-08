PulseAudioComboBox {
	id: comboBox
	updateModel: function(){
		fakelibQmlInterface.updateSourceOuputsList()
		let sourceOutputsCount = fakelibQmlInterface.sourceOutputsCount()
		model.clear()
		if (sourceOutputsCount === 0) {
			model.append({ "name": "",
				"description": qsTr("No Source Output Available"),
				"index": -1, "isChecked": false})
		}
		for (let i = 0; i < sourceOutputsCount; ++i) {
			let sourceOutput = fakelibQmlInterface.sourceOutputAt(i)
			let isChecked = false
			if (root.sourceOutputIndex === sourceOutput.index) {
				isChecked = true
			}
			model.append({"name": sourceOutput.name,
						   "description": sourceOutput.processBinaryName,
						   "index": sourceOutput.index, "isChecked": isChecked})
		}
		currentIndex = 0
	}
}
