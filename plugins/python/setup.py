from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext
from Cython.Build import cythonize

INCL_DIRS = [ '../../include', '../../dataCollect/events', '../../external/plf_colony', '../../dataCollect/model', '../../dataCollect']
src_cycle = [ 'Cycle.pyx', '../../dataCollect/model/Cycle.cpp', '../../dataCollect/model/Packet.cpp', '../../dataCollect/model/ODDescription.cpp', '../../dataCollect/model/PacketDiff.cpp', '../../dataCollect/model/ODEntry.cpp', '../../dataCollect/model/OD.cpp', '../../dataCollect/model/Node.cpp', '../../dataCollect/model/ODEntryContainer.cpp']
src_api = [ 'PluginAPI.pyx', 'PythonPlugin.cpp', '../../dataCollect/PluginBase.cpp', '../../dataCollect/events/EventBase.cpp', '../../dataCollect/model/Cycle.cpp', '../../dataCollect/model/Packet.cpp', '../../dataCollect/model/ODDescription.cpp', '../../dataCollect/model/PacketDiff.cpp', '../../dataCollect/model/ODEntry.cpp', '../../dataCollect/model/OD.cpp', '../../dataCollect/model/Node.cpp', '../../dataCollect/model/ODEntryContainer.cpp', '../../dataCollect/EPLEnum2Str.cpp', 'CSPythonPluginStorage.cpp', '../../dataCollect/model/CycleStorageBase.cpp' ]
## OPTIONS
# Cycle.pyx options
CYC_EXT = [Extension('Cycle',
      sources = src_cycle,
      include_dirs = INCL_DIRS,
      extra_compile_args = [ '-std=c++14' ],
      language = 'c++',)
]
# PluginAPI.pyx options
API_EXT = [Extension('PluginAPI',
      sources = src_api,
      include_dirs = INCL_DIRS,
      extra_compile_args = [ '-std=c++14' ],
      language = 'c++',)
]
# Plugin.pyx options
PLU_EXT = [Extension('Plugin',
      sources = ["Plugin.pyx"],
      include_dirs = INCL_DIRS,
      extra_compile_args = [ '-std=c++14' ],
      language = 'c++',)
]

#GUI API
setup(
  name = 'PluginGUIAPI',
  ext_modules=[
    Extension('PluginGUIAPI', ['PluginGUIAPI.pyx'], language="c++")
    ],
  cmdclass = {'build_ext': build_ext}
)


## SETUPS
setup(ext_modules = cythonize(CYC_EXT))
setup(ext_modules = cythonize(API_EXT))
setup(ext_modules = cythonize(PLU_EXT))

