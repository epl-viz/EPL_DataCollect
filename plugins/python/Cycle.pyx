cimport CCycle
from libcpp.vector cimport vector

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

  def getActiveEvents(self):
    listOfEvents = []
    cdef vector[CCycle.EventBase *] eventVector = self._C_Cycle.getActiveEvents()
    for event in eventVector:
      pyEvent = (event.getTypeAsString(), event.getName(), event.getDescription(), event.getEventFlags())
      listOfEvents.append(pyEvent)
    return listOfEvents
