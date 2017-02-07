from libcpp.vector cimport vector
from libcpp.string cimport string
from libcpp cimport bool

cdef extern from "../../dataCollect/model/Cycle.hpp" namespace "EPL_DataCollect":
	cdef cppclass Cycle:
		Cycle() except +
		int getNumNodes()
		int getCycleNum()
		vector[EventBase *] getActiveEvents()
		Node getNode(int)
		bool registerCycleStorage(string, CycleStorageBase)
		CycleStorageBase* getCycleStorage(string)

cdef extern from "../../dataCollect/events/EventBase.hpp" namespace "EPL_DataCollect":
	cdef cppclass EventBase:
		EventBase() except +
		
cdef extern from "../../dataCollect/model/Node.hpp" namespace "EPL_DataCollect":
	cdef cppclass Node:
		Node() except +
		
cdef extern from "../../dataCollect/model/CycleStorageBase.hpp" namespace "EPL_DataCollect":
	cdef cppclass CycleStorageBase:
		CycleStorageBase() except +
