# This is just a file to expose cython stuff to other cython stuff
cimport CCycle

cdef createCycle(CCycle.Cycle*)
