PulseAudioComboBox {
	id: comboBox
	model: fakelibQmlInterface.sources
	updateModel: function(){
		fakelibQmlInterface.updateSourcesList()
		currentIndex = 0
	}
}
