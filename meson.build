project('ControlsForFake', 'cpp', default_options : ['cpp_std=c++17'])

qt5 = import('qt5')
qt5_dep = dependency('qt5', modules: [ 'Core', 'Gui', 'Widgets', 'Qml', 'Quick' ], required: true)
libpulse_dep = dependency('libpulse')
libFakeMicWavPlayer_dep = dependency('fakemicwavplayer', required: true)
QQmlObjectListModel_proj = subproject('qt-qml-models')
QQmlObjectListModel_dep = QQmlObjectListModel_proj.get_variable('lib_dep')

dependencies = [ qt5_dep, libpulse_dep, libFakeMicWavPlayer_dep, QQmlObjectListModel_dep ]

inc = include_directories('include')

# translations = qt5.compile_translations(
#   ts_files : [ 'translations/ControlsForFake_fr_FR.ts' ],
#   install: true,
#   install_dir: 'translations',
#   build_by_default : true)


moc_files = qt5.preprocess(moc_headers : 
						   [ 'include/fakelibqmlinterface.h', 
							 'include/fakeplayerthread.h',
							 'include/SubscribeAndListenThread.h',
							 'include/sink.h', 
							 'include/source.h', 
							 'include/sourceoutput.h',
							 'include/sinkinput.h' 
							],
						   include_directories: inc,
						   dependencies: dependencies,
						   qresources: [ 'qml.qrc', 'icons.qrc', 'translations.qrc' ])



sources = [
			'SubscribeAndListenThread.cpp',
			'fakelibqmlinterface.cpp',
			'fakeplayerthread.cpp',
			'main.cpp',
			'sink.cpp',
			'sinkinput.cpp',
			'source.cpp',
			'sourceoutput.cpp',
		  ]

executable('ControlsForFake', 
		sources,
	    moc_files,
	    install: true,
		include_directories: inc,
		dependencies: dependencies,
		)

conf_data = configuration_data()
conf_data.set('bindir', get_option('prefix')+'/'+get_option('bindir'))
desktop_file = configure_file(input : 'desktop/controls-for-fake.desktop.in',
						      output : 'controls-for-fake.desktop',
				              configuration : conf_data)

install_data('icons/16-app-icon.png',
	      install_dir: 'share/icons/hicolor/16x16/apps',
	      rename: 'controlsforfake.png')
install_data('icons/22-app-icon.png',
	      install_dir: 'share/icons/hicolor/22x22/apps',
	      rename: 'controlsforfake.png')
install_data('icons/32-app-icon.png',
	      install_dir: 'share/icons/hicolor/32x32/apps',
	      rename: 'controlsforfake.png')
install_data('icons/48-app-icon.png',
	      install_dir: 'share/icons/hicolor/48x48/apps',
	      rename: 'controlsforfake.png')
install_data('icons/app-icon.svgz',
	      install_dir: 'share/icons/hicolor/scalable/apps',
	      rename: 'controlsforfake.svgz')

install_data(desktop_file,
	      install_dir: 'share/applications',
	      install_mode: 'r-xr-xr-x')
