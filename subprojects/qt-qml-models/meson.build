project('QQmlObjectListModel', 'cpp', default_options : ['cpp_std=c++17'])

qt5 = import('qt5')
qt5_dep = dependency('qt5', modules: [ 'Core', 'Qml' ], required: true)

inc = include_directories('.')
headers = [ 
			'QQmlObjectListModel.h',
			'QQmlVariantListModel.h',
		  ]
moc_files = qt5.preprocess(moc_headers : headers,
						   include_directories: inc,
						   dependencies: qt5_dep)

sources = [
		  'QQmlObjectListModel.cpp' ,
		  'QQmlVariantListModel.cpp',
		  ]
lib = shared_library('QQmlObjectListModel', sources, 
  moc_files,
  include_directories : inc,
  dependencies : [qt5_dep], 
  install : true)

lib_dep = declare_dependency(
  link_with: lib,
  include_directories: inc)
