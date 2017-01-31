cdef extern from "../../dataCollect/model/Cycle.hpp" namespace "EPL_DataCollect":
	cdef cppclass Cycle:
		Cycle() except +
		int getCycleNum()
