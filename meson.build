project('ControlsForFake', 'cpp')

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



lib_fake_dep = dependency('fake', required: true)

libpulse_simple = dependency('libpulse-simple')

executable('ControlsForFake', 
	    [ 'main.cpp', 'fakelibqmlinterface.cpp', 'sink.cpp', 'sourceoutput.cpp' ],
	    moc_files,
	    install: true,
            include_directories: inc,
            dependencies : [ qt5_dep, libpulse_simple, lib_fake_dep ])
