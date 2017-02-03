# distutils: include_dirs = [../../include, ../../dataCollect/events, ../../external/plf_colony, ../../dataCollect/model]

cdef extern from "../../dataCollect/model/Cycle.hpp" namespace "EPL_DataCollect":
	cdef cppclass Cycle:
		Cycle() except +
