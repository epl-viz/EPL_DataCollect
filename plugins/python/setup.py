from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext
from Cython.Build import cythonize

#Backend API
setup(
  name = 'PluginAPI',
  ext_modules=[
    Extension('PluginAPI', ['PluginAPI.pyx'])
    ],
  cmdclass = {'build_ext': build_ext}
)

#GUI API
setup(
  name = 'PluginGUIAPI',
  ext_modules=[
    Extension('PluginGUIAPI', ['PluginGUIAPI.pyx'])
    ],
  cmdclass = {'build_ext': build_ext}
)

#Wrapper for Cycle class
setup(ext_modules = cythonize(
		"Cyc.pyx",
		include_path = [],
		language = "c++",
	))
