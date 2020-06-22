project('ControlsForFake', 'cpp', default_options : ['cpp_std=c++17'])

qt5 = import('qt5')
qt5_dep = dependency('qt5', modules: [ 'Core', 'Gui', 'Widgets', 'Qml', 'Quick' ], required: true)

inc = include_directories('include')

# translations = qt5.compile_translations(
#   ts_files : [ 'translations/ControlsForFake_fr_FR.ts' ],
#   install: true,
#   install_dir: 'translations',
#   build_by_default : true)

moc_files = qt5.preprocess(moc_headers : 
			     [ 'include/fakelibqmlinterface.h', 
			       'include/sink.h', 
			       'include/sinklist.h', 
			       'include/sourceoutput.h' ],
                           include_directories: inc,
                           dependencies: qt5_dep,
			   qresources: [ 'qml.qrc', 'icons.qrc', 'translations.qrc' ])

libfake_dep = dependency('fake', required: true)
libFakeMicWavPlayer_dep = dependency('fakemicwavplayer', required: true)
libOggPlayer_dep = dependency('oggplayer', required: true)
libvorbis = dependency('vorbis', required: true)
libogg = dependency('ogg', required: true)

libpulse_simple = dependency('libpulse-simple')

executable('ControlsForFake', 
	    [ 'main.cpp', 'fakelibqmlinterface.cpp', 'sink.cpp', 'sourceoutput.cpp' ],
	    moc_files,
	    install: true,
            include_directories: inc,
            dependencies : [ qt5_dep, libpulse_simple, libvorbis, libogg, libOggPlayer_dep, libFakeMicWavPlayer_dep, libfake_dep ])


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

install_data('controls-for-fake.desktop',
	      install_dir: 'share/applications',
	      install_mode: 'r-xr-xr-x')
