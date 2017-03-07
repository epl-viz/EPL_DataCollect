from libcpp.vector cimport vector
from libcpp.string cimport string
from libcpp cimport bool
from libcpp.unordered_map cimport unordered_map

#Cycle specific declarations
#################################################################################
cdef extern from "Cycle.hpp" namespace "EPL_DataCollect":
  cdef cppclass Cycle:
    Cycle() except +
    int getNumNodes()
    int getCycleNum()
    vector[EventBase *] getActiveEvents()
    Node* getNode(int)
    CycleStorageBase* getCycleStorage(string)
    ODEntry* getODEntry(int, int)

    bool registerCycleStorage(string, CycleStorageBase)


cdef extern from "Node.hpp" namespace "EPL_DataCollect":
  cdef cppclass Node:
    OD* getOD()
    int getID()
    string getStatusStr()


cdef extern from "OD.hpp" namespace "EPL_DataCollect":
  cdef cppclass OD:
    ODEntry* getEntry(int)


cdef extern from "ODEntry.hpp" namespace "EPL_DataCollect":
  cdef cppclass ODEntry:
    string toString(unsigned int)


cdef extern from "EventBase.hpp" namespace "EPL_DataCollect":
  cdef cppclass EventBase:
    string getTypeAsString()
    string getName()
    string getDescription()
    string getPluginID()
    int getEventFlags()


cdef extern from "CycleStorageBase.hpp" namespace "EPL_DataCollect":
  cdef cppclass CycleStorageBase:
    CycleStorageBase() except +
    string getStringRepresentation()
