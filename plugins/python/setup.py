from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext
from Cython.Build import cythonize

#Wrapper for Cycle class
src = [ 'Cycle.pyx', '../../dataCollect/model/Cycle.cpp', '../../dataCollect/model/Packet.cpp', '../../dataCollect/model/ODDescription.cpp', '../../dataCollect/model/PacketDiff.cpp', '../../dataCollect/model/ODEntry.cpp', '../../dataCollect/model/OD.cpp', '../../dataCollect/model/Node.cpp', '../../dataCollect/model/ODEntryContainer.cpp']
extension = [Extension('Cycle',
			sources = src,
			include_dirs = ['../../include', '../../dataCollect/events', '../../external/plf_colony', '../../dataCollect/model'],
			extra_compile_args = [ '-std=c++14' ],
			language = 'c++',)
]

#Wrapper for Cycle class
setup(ext_modules = cythonize(extension))

#Plugin Wrapper
setup(
	name = 'Plugin',
	ext_modules = [ 
		Extension("Plugin",
           		sources=["Plugin.pyx"],  # additional source file(s)
           		language="c++"),             # generate C++ code
      	],
	cmdclass = {'build_ext': build_ext}
)

#Backend API
setup(
  name = 'PluginAPI',
  ext_modules=[
    Extension('PluginAPI', ['PluginAPI.pyx'], language="c++")
    ],
  cmdclass = {'build_ext': build_ext}
)

#GUI API
setup(
  name = 'PluginGUIAPI',
  ext_modules=[
    Extension('PluginGUIAPI', ['PluginGUIAPI.pyx'], language="c++")
    ],
  cmdclass = {'build_ext': build_ext}
)



