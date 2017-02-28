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
    ODEntry* getODEntry(int, int)

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
    string toString()


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
    string getStringRepresentation()

cdef extern from "../../include/EPLEnums.h":
  ctypedef enum EvType:
    PROTO_ERROR,
    ERROR,
    WARNING,
    INFO,
    DEBUG,

    # plugin specific events
    PLUGIN_OTHER,
    PLUGIN_EV_TEXT,

    # view specific events
    VIEW_STARTCAP,
    VIEW_ENDCAP,
    VIEW_EV_HIGHLIGHT_MN,
    VIEW_EV_HIGHLIGHT_CN,
    VIEW_EV_JUMPTOTIME,
    VIEW_EV_HIGHLIGHT_OD_ENTRY,
    VIEW_EV_TEXT,
    VIEW_EV_IMAGE,

    FRONTEND_OTHER,

    UNKNOWN
