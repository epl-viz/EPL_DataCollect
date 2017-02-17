from libcpp.vector cimport vector
from libcpp.string cimport string
from libcpp cimport bool
from libcpp.unordered_map cimport unordered_map

cdef extern from "../../dataCollect/model/Cycle.hpp" namespace "EPL_DataCollect":
  cdef cppclass Cycle:
    Cycle() except +
    int getNumNodes()
    int getCycleNum()
    vector[EventBase *] getActiveEvents()
    Node* getNode(int)
    bool registerCycleStorage(string, CycleStorageBase)
    CycleStorageBase* getCycleStorage(string)

cdef extern from "../../dataCollect/model/Node.hpp" namespace "EPL_DataCollect":
  cdef cppclass Node:
    Node() except+
    OD* getOD()
    int getID()
    string getStatusStr()

cdef extern from "../../dataCollect/model/OD.hpp" namespace "EPL_DataCollect":
  cdef cppclass OD:
    OD() except+
    ODEntry* getEntry(int)

cdef extern from "../../dataCollect/model/ODEntry.hpp" namespace "EPL_DataCollect":
  cdef cppclass ODEntry:
    ODEntry() except+
    #TODO: Methods
    #       string getEntry_AsString(int)
    #       int getEntry_AsInt(int)
    #       int getEntry_AsUInt(int)
    #       bool getEntry_AsBool(int)
    #       int getEntry_AsComplex(int)


cdef extern from "../../dataCollect/events/EventBase.hpp" namespace "EPL_DataCollect":
  cdef cppclass EventBase:
    EventBase(EvType, string, string, string, string, int, Cycle*, unordered_map[int, string]) except +
    string getTypeAsString()
    string getName()
    string getDescription()
    string getPluginID()
    int getEventFlags()



cdef extern from "../../dataCollect/model/CycleStorageBase.hpp" namespace "EPL_DataCollect":
  cdef cppclass CycleStorageBase:
    CycleStorageBase() except +

cdef extern from "../../include/EPLEnums.h":
  ctypedef enum EvType:
    EVT_PROTO_ERROR,
    EVT_ERROR,
    EVT_WARNING,
    EVT_INFO,
    EVT_DEBUG,
    EVT_PLUGIN_EV_TEXT,
    EVT_PLUGIN_OTHER = 0x1000,
    EVT_FRONTEND_OTHER = 0x2000,
    EVT_UNKNOWN
