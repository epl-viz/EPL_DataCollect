# distutils: include_dirs = [../../include, ../../dataCollect/events, ../../external/plf_colony, ../../dataCollect/model]
#clib
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
		
cdef class PyCycle:
	cdef Cycle* c_cycle	# hold c++ instance
	def __cinit__(self):
		self.c_cycle = new Cycle()
	def __dealloc__(self):
		del self.c_cycle

cdef extern from "../../dataCollect/events/EventBase.hpp" namespace "EPL_DataCollect":
	cdef cppclass EventBase:
		EventBase() except +
		
cdef extern from "../../dataCollect/model/Node.hpp" namespace "EPL_DataCollect":
	cdef cppclass Node:
		Node() except +
		
cdef extern from "../../dataCollect/model/CycleStorageBase.hpp" namespace "EPL_DataCollect":
	cdef cppclass CycleStorageBase:
		CycleStorageBase() except +
