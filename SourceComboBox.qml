PulseAudioComboBox {
	id: comboBox
	updateModel: function(){
		fakelibQmlInterface.updateSourcesList()
		let sourcesCount = fakelibQmlInterface.sourceCount()
		model.clear()
		for (let i = 0; i < sourcesCount; ++i) {
			let source = fakelibQmlInterface.sourceAt(i)
			let isChecked = source.index === root.sourceIndex
			model.append({ "description":source.description,
			"name":source.name,
			"index": source.index,
			"isChecked": isChecked})
		}
		currentIndex = 0
	}
}
