cimport CCycle
from libcpp.string cimport string

cdef extern from "../../dataCollect/PluginBase.hpp" namespace "EPL_DataCollect":
  cdef cppclass PluginBase:
    PluginBase() except +
    void run(CCycle.Cycle*)
    string getDependencies()
    string getID()
    addEvent(CCycle.EventBase*)
    #registerCycleStorage but how ?

cdef extern from "PythonPlugin.hpp" namespace "EPL_DataCollect::plugins":
  cdef cppclass PythonPlugin(PluginBase):
    PythonPlugin() except +
