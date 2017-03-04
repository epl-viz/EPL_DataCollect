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
    bool registerPyCycleStorage(string, int)
    bool setStorage(string, string)
    string getStorage(string)
    string getData(string)
    bool setDataStr(string, string)
    bool setDataInt(string, int)
    CCycle.Cycle* getCycleByNum(int)
    CCycle.Cycle* getCurrentCycle()
    CaptureInstance* getCI()

    @staticmethod
    PythonPlugin* getPythonPlugin(string)



## Helper stuff that is not exposed directly to python
cdef extern from "CaptureInstance.hpp" namespace "EPL_DataCollect":
  cdef cppclass CaptureInstance:
    CycleContainer* getCycleContainer()

cdef extern from "CycleContainer.hpp" namespace "EPL_DataCollect":
  cdef cppclass CycleContainer:
    CCycle.Cycle getCycle(int)
