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
    string getData(const char*)
    bool setDataStr(const char*, const char*)
    bool setDataInt(const char*, int)

    @staticmethod
    CCycle.Cycle* getCurrentCycle()
    @staticmethod
    PythonPlugin* getPythonPlugin(const char*)
    @staticmethod
    CCycle.Cycle* getCycleWithNum(const char*, int)

    CaptureInstance* getCI()

## Helper stuff that is not exposed directly to python
cdef extern from "CaptureInstance.hpp" namespace "EPL_DataCollect":
  cdef cppclass CaptureInstance:
    CycleContainer* getCycleContainer()

cdef extern from "CycleContainer.hpp" namespace "EPL_DataCollect":
  cdef cppclass CycleContainer:
    CCycle.Cycle getCycle(int)
