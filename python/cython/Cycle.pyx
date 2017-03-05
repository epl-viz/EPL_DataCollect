cimport CCycle
cimport CPlugin
from libcpp.vector cimport vector
from libcpp.string cimport string
from libcpp cimport bool
from libcpp.unordered_map cimport unordered_map

cdef class Cycle:
  cdef CCycle.Cycle* _C_Cycle

  def __cinit__(self):
    pass

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

  def getNodeStatus(self, nodeNumber):
    if not (isinstance(nodeNumber, int)):   # numbers have to be integer
      return
    if nodeNumber < 0 or nodeNumber >= self.getNumNodes():                # and in correct size
      return
    return self._C_Cycle.getNode(nodeNumber).getStatusStr()

  def getODEntry(self, nodeNumber, odNumber):
    if not (isinstance(nodeNumber, int) and isinstance(odNumber, int)):   # numbers have to be integer
      return
    if nodeNumber < 0:                # and in correct size
      return
    if odNumber < 0x0000 or odNumber >= 0xFFFF:                           # od entry available
      return
    cdef CCycle.ODEntry* odEntry = self._C_Cycle.getODEntry(nodeNumber, odNumber)
    if (odEntry != NULL):
      return odEntry.toString()

  def getAmountOfCN(self):
    """
    \brief This method returns the amount of controlled nodes currently active. The amount of controlled nodes is retrieved from a specific cycle, thus representing the CNs active at that given cycle.

    \returns number of CNs in the current cycle
    """
    return self._C_Cycle.getNumNodes()

# creating a cycle from Cycle* ...
cdef createCycle(CCycle.Cycle* curCycle):
  cyc = Cycle()
  cyc._C_Cycle = curCycle
  return cyc
