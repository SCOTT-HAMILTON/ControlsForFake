project('ControlFakeMicWavPlayer', 'cpp')

qt5 = import('qt5')
qt5_dep = dependency('qt5', modules: [ 'Core', 'Gui', 'Widgets', 'Qml' ], required: true)

inc = include_directories('include')
if get_option('qt5_include_dir') != ''
  inc = include_directories('include', get_option('qt5_include_dir')+'/QtCore')
endif

moc_headers = [ 'include/fakelibqmlinterface.h', 'include/sink.h', 'include/sinklist.h',  'include/sourceoutput.h' ]
sources = [ 'main.cpp', 'fakelibqmlinterface.cpp', 'sink.cpp', 'sourceoutput.cpp' ]
moc_files = qt5.preprocess(moc_headers : moc_headers,
                           include_directories: inc,
			   moc_extra_arguments: ['-DMAKES_MY_MOC_HEADER_COMPILE'],
                           dependencies: qt5_dep,
			   qresources: [ 'qml.qrc', 'icons.qrc' ])
translations = qt5.compile_translations(ts_files : 'ControlFakeMicWavePlayer_fr_FR.ts', build_by_default : true)


lib_fake_dep = dependency('fake', required: true)

libpulse_simple = dependency('libpulse-simple')

executable('ControlFakeMicWavPlayer', 
	    sources,
	    moc_files,
	    install: true,
            include_directories: inc,
            dependencies : [ qt5_dep, libpulse_simple, lib_fake_dep ])
