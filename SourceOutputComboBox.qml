PulseAudioComboBox {
	id: comboBox
	model: fakelibQmlInterface.sourceOutputs
	textRole: "processBinaryName"
	updateModel: function(){
		fakelibQmlInterface.updateSourceOutputsList()
		currentIndex = 0
	}
}
