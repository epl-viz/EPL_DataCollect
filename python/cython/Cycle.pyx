cimport CCycle
cimport CPlugin
from libcpp.vector cimport vector
from libcpp.string cimport string
from libcpp cimport bool
from libcpp.unordered_map cimport unordered_map

cdef int ERRVAL = -1

cdef class Cycle:
  cdef CCycle.Cycle* _C_Cycle

  def __cinit__(self):
    self._C_Cycle = CPlugin.PythonPlugin.getCurrentCycle()
    if self._C_Cycle is NULL:
      raise MemoryError()

  cpdef int getNumNodes(self):
    return self._C_Cycle.getNumNodes()

  cpdef int getCycleNum(self):
    return self._C_Cycle.getCycleNum()

  cdef void updateCycle(self, CCycle.Cycle* newCycle):  ## maybe implement this since it increases speed
    if newCycle != NULL:
      self._C_Cycle = newCycle

  def getActiveEvents(self):
    listOfEvents = []
    cdef vector[CCycle.EventBase *] eventVector = self._C_Cycle.getActiveEvents()
    for event in eventVector:
      pyEvent = (event.getTypeAsString(), event.getName(), event.getDescription(), event.getEventFlags())
      listOfEvents.append(pyEvent)
    return listOfEvents

  def getODEntry(self, nodeNumber, odnumber):
    if not (isinstance(nodeNumber, int) and isinstance(odnumber, int)):   # numbers have to be integer
      return ERRVAL
    if nodeNumber < 0 or nodeNumber >= self.getNumNodes():                # and in correct size
      return ERRVAL
    if odnumber < 0x0000 or odnumber >= 0xFFFF:                           # od entry available
      return ERRVAL
    return 42 ##TODO Implement with cycle.getODEntry(...)

  def getNodeStatus(self, nodeNumber):
    if not (isinstance(nodeNumber, int)):   # numbers have to be integer
      return ERRVAL
    if nodeNumber < 0 or nodeNumber >= self.getNumNodes():                # and in correct size
      return ERRVAL
    return 42 ##TODO Implement with cycle.getNodeStatus(...)
