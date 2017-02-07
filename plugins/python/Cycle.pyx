cimport CCycle

cdef class Cycle:
	cdef CCycle.Cycle* _C_Cycle
	def __cinit__(self):
		self._C_Cycle = new CCycle.Cycle()
		if self._C_Cycle is NULL:
			raise MemoryError()
	cpdef int getNumNodes(self):
		return self._C_Cycle.getNumNodes()
	cpdef int getCycleNum(self):
		return self._C_Cycle.getCycleNum()
	cpdef int getANum(self):
		return 42
