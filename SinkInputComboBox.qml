PulseAudioComboBox {
	id: comboBox
	model: fakelibQmlInterface.sinkInputs
	textRole: "processBinaryName"
	updateModel: function(){
		fakelibQmlInterface.updateSinkInputsList()
		currentIndex = 0
	}
}
