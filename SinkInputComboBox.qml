PulseAudioComboBox {
	id: comboBox
	updateModel: function(){
		fakelibQmlInterface.updateSinkInputsList()
		let sinkInputsCount = fakelibQmlInterface.sinkInputsCount()
		model.clear()
		if (sinkInputsCount === 0) {
			model.append({ "name": "",
				"description": qsTr("No application available"),
				"index": -1, "isChecked": false})
		}
		for (let i = 0; i < sinkInputsCount; ++i) {
			let sinkInput = fakelibQmlInterface.sinkInputAt(i)
			let isChecked = false
			if (root.sinkInputIndex === sinkInput.index) {
				isChecked = true
			}
			model.append({"name": sinkInput.name,
						   "description": sinkInput.processBinaryName,
						   "index": sinkInput.index, "isChecked": isChecked})
		}
		currentIndex = 0
	}
}
