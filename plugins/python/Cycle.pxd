from libcpp.vector cimport vector
from libcpp.string cimport string
from libcpp cimport bool

# DEFINITION FILE!

cdef cppclass Cycle:
	Cycle() except +
	int getNumNodes()
	int getCycleNum()
	vector[EventBase *] getActiveEvents()
	Node getNode(int)
	bool registerCycleStorage(string, CycleStorageBase)
	CycleStorageBase* getCycleStorage(string)
		
cdef cppclass EventBase:
	EventBase() except +
		
cdef cppclass Node:
	Node() except +
		
cdef cppclass CycleStorageBase:
	CycleStorageBase() except +
