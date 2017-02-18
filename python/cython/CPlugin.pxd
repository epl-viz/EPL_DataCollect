cimport CCycle
from libcpp.string cimport string
from libcpp cimport bool

cdef extern from "../../dataCollect/PluginBase.hpp" namespace "EPL_DataCollect":
  cdef cppclass PluginBase:
    string getDependencies()
    string getID()

cdef extern from "PythonPlugin.hpp" namespace "EPL_DataCollect::plugins":
  cdef cppclass PythonPlugin(PluginBase):
    bool addPyEvent(int, const char*)
    bool registerPyCycleStorage(const char*, int)
    bool setStorage(const char*, const char*)
    string getStorage(const char*)

    @staticmethod
    CCycle.Cycle* getCurrentCycle()
    @staticmethod
    PythonPlugin* getPythonPlugin(const char*)
