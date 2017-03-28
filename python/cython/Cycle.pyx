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
    """
    \brief This method returns the amount of controlled nodes currently active. The amount of controlled nodes is retrieved from a specific cycle, thus representing the CNs active at that given cycle.

    \returns number of CNs in the current cycle
    """
    return self._C_Cycle.getNumNodes()

  cpdef int getCycleNum(self):
    """
    \returns number of the current cycle
    """
    return self._C_Cycle.getCycleNum()

  cdef void updateCycle(self, CCycle.Cycle* newCycle):  ## maybe implement this since it increases speed
    if newCycle != NULL:
      self._C_Cycle = newCycle

  def getActiveEvents(self):
    """
    \brief getting the active events of the cycle, note that this is mostly empty and only contains data of python plugins.
          This method is used if you try to get old cycles and check for their events

    \returns touple (type as int, string type as str, name of event, description, integer flag)
    """
    listOfEvents = []
    cdef vector[CCycle.EventBase *] eventVector = self._C_Cycle.getActiveEvents()
    for event in eventVector:
      pyEvent = (<int>event.getType(), event.getTypeAsString().decode(), event.getName().decode(), event.getDescription().decode(), event.getEventFlags())
      listOfEvents.append(pyEvent)
    return listOfEvents

  def getNodeStatus(self, nodeNumber):
    """
    \brief returns status of Node as str None if node not available, otherwise
            { OFF, PRE_OPERATIONAL_2, READY, OPERATIONAL... (the POWERLINK states)}

    \param nodeNumber number of the node

    \returns str representation of od entry
    """
    if not (isinstance(nodeNumber, int)):   # numbers have to be integer
      return
    if nodeNumber < 0:                # and in correct size
      return
    if self._C_Cycle.getNode(nodeNumber) == NULL:
      return
    return self._C_Cycle.getNode(nodeNumber).getStatusStr().decode()

  def getODEntry(self, nodeNumber, odNumber):
    """
    \brief This method returns the ODEntry, subindex 0 as string represented. None if not available, standard values are 0 or ""<N/A>" on complex types

    \param nodeNumber number of the node
    \param odNumber number of the ODEntry

    \returns str representation of od entry
    """
    if not (isinstance(nodeNumber, int) and isinstance(odNumber, int)):   # numbers have to be integer
      return
    if nodeNumber < 0:                # and in correct size
      return
    if odNumber < 0x0000 or odNumber >= 0xFFFF:                           # od entry available
      return
    cdef CCycle.ODEntry* odEntry = self._C_Cycle.getODEntry(nodeNumber, odNumber)
    if (odEntry != NULL):
      return odEntry.toString(0).decode()

  def getODEntry_Sub(self, nodeNumber, odNumber, subindex):
    """
    \brief This method returns the ODEntry, subindex as string represented. None if not available, standard values are 0 or ""<N/A>" on complex types

    \param nodeNumber number of the node
    \param odNumber number of the ODEntry
    \param subindex subindex number

    \returns str representation of od entry
    """
    if not (isinstance(nodeNumber, int) and isinstance(odNumber, int) and isinstance(subindex, int)):   # numbers have to be integer
      return
    if nodeNumber < 0:                # and in correct size
      return
    if odNumber < 0x0000 or odNumber >= 0xFFFF:                           # od entry available
      return
    cdef CCycle.ODEntry* odEntry = self._C_Cycle.getODEntry(nodeNumber, odNumber)
    if (odEntry != NULL):
      return odEntry.toString(subindex).decode()

  def getAmountOfCN(self):
    """
    \brief This method returns the amount of controlled nodes currently active. The amount of controlled nodes is retrieved from a specific cycle, thus representing the CNs active at that given cycle.

    \returns number of CNs in the current cycle
    """
    return self._C_Cycle.getNumNodes()

  ## getter methods for getting the commonly used node stuff, None if the node is not available !
  # str returned by:
  def getDeviceType(self, nodeNumber):
    """
    \brief This method returns the device type of a node.

    \param nodeNumber the number of the node

    \returns device type of CNs in the current cycle as str
    """
    if not (isinstance(nodeNumber, int)):   # numbers have to be integer
      return
    if nodeNumber < 0:                # and in correct size
      return
    if self._C_Cycle.getNode(nodeNumber) == NULL:
      return
    return self._C_Cycle.getNode(nodeNumber).getDeviceType().decode()

  def getIPAddress(self, nodeNumber):
    """
    \brief This method returns the ip address of a node.

    \param nodeNumber the number of the node

    \returns ip address of CNs in the current cycle as str
    """
    if not (isinstance(nodeNumber, int)):   # numbers have to be integer
      return
    if nodeNumber < 0:                # and in correct size
      return
    if self._C_Cycle.getNode(nodeNumber) == NULL:
      return
    return self._C_Cycle.getNode(nodeNumber).getIPAddress().decode()

  def getSubnetMask(self, nodeNumber):
    """
    \brief This method returns the subnet mask of a node.

    \param nodeNumber the number of the node

    \returns subnet mask of CNs in the current cycle as str
    """
    if not (isinstance(nodeNumber, int)):   # numbers have to be integer
      return
    if nodeNumber < 0:                # and in correct size
      return
    if self._C_Cycle.getNode(nodeNumber) == NULL:
      return
    return self._C_Cycle.getNode(nodeNumber).getSubnetMask().decode()

  def getDefaultGateway(self, nodeNumber):
    """
    \brief This method returns the default gateway of a node.

    \param nodeNumber the number of the node

    \returns default gateway of CNs in the current cycle as str
    """
    if not (isinstance(nodeNumber, int)):   # numbers have to be integer
      return
    if nodeNumber < 0:                # and in correct size
      return
    if self._C_Cycle.getNode(nodeNumber) == NULL:
      return
    return self._C_Cycle.getNode(nodeNumber).getDefaultGateway().decode()

  def getHostName(self, nodeNumber):
    """
    \brief This method returns the host name of a node.

    \param nodeNumber the number of the node

    \returns host name of CNs in the current cycle as str
    """
    if not (isinstance(nodeNumber, int)):   # numbers have to be integer
      return
    if nodeNumber < 0:                # and in correct size
      return
    if self._C_Cycle.getNode(nodeNumber) == NULL:
      return
    return self._C_Cycle.getNode(nodeNumber).getHostName().decode()

  # int returned by:
  def getProfile(self, nodeNumber):
    """
    \brief This method returns the profile of a node.

    \param nodeNumber the number of the node

    \returns profile of CNs in the current cycle as str
    """
    if not (isinstance(nodeNumber, int)):   # numbers have to be integer
      return
    if nodeNumber < 0:                # and in correct size
      return
    if self._C_Cycle.getNode(nodeNumber) == NULL:
      return
    return self._C_Cycle.getNode(nodeNumber).getProfile()

  def getVendorId(self, nodeNumber):
    """
    \brief This method returns the vendor id of a node.

    \param nodeNumber the number of the node

    \returns vendor id of CNs in the current cycle as int
    """
    if not (isinstance(nodeNumber, int)):   # numbers have to be integer
      return
    if nodeNumber < 0:                # and in correct size
      return
    if self._C_Cycle.getNode(nodeNumber) == NULL:
      return
    return self._C_Cycle.getNode(nodeNumber).getVendorId()

  def getProductCode(self, nodeNumber):
    """
    \brief This method returns the product code of a node.

    \param nodeNumber the number of the node

    \returns product code of CNs in the current cycle as int
    """
    if not (isinstance(nodeNumber, int)):   # numbers have to be integer
      return
    if nodeNumber < 0:                # and in correct size
      return
    if self._C_Cycle.getNode(nodeNumber) == NULL:
      return
    return self._C_Cycle.getNode(nodeNumber).getProductCode()

  def getRevisionNumber(self, nodeNumber):
    """
    \brief This method returns the revision number of a node.

    \param nodeNumber the number of the node

    \returns revision number of CNs in the current cycle as int
    """
    if not (isinstance(nodeNumber, int)):   # numbers have to be integer
      return
    if nodeNumber < 0:                # and in correct size
      return
    if self._C_Cycle.getNode(nodeNumber) == NULL:
      return
    return self._C_Cycle.getNode(nodeNumber).getRevisionNumber()

  def getSerialNumber(self, nodeNumber):
    """
    \brief This method returns the serial number of a node.

    \param nodeNumber the number of the node

    \returns serial number of CNs in the current cycle as int
    """
    if not (isinstance(nodeNumber, int)):   # numbers have to be integer
      return
    if nodeNumber < 0:                # and in correct size
      return
    if self._C_Cycle.getNode(nodeNumber) == NULL:
      return
    return self._C_Cycle.getNode(nodeNumber).getSerialNumber()

  def getResponseTime(self, nodeNumber):
    """
    \brief This method returns the response time of a node.

    \param nodeNumber the number of the node

    \returns response time of CNs in the current cycle as int
    """
    if not (isinstance(nodeNumber, int)):   # numbers have to be integer
      return
    if nodeNumber < 0:                # and in correct size
      return
    if self._C_Cycle.getNode(nodeNumber) == NULL:
      return
    return self._C_Cycle.getNode(nodeNumber).getResponseTime()

  # packet specific methods, returning None if no package of this type in cycle
  def getPacketSrc(self, nodeNumber):
    """
    \brief This method returns the device type of a node.

    \param nodeNumber the number of the node

    \returns number of CNs in the current cycle
    """
    if not (isinstance(nodeNumber, int)):   # numbers have to be integer
      return
    if nodeNumber < 0:                # and in correct size
      return
    if self._C_Cycle.getPacket(nodeNumber) == NULL:
      return
    return self._C_Cycle.getPacket(nodeNumber).getSrcNode()

  def getPacketDest(self, nodeNumber):
    """
    \brief This method returns the device type of a node.

    \param nodeNumber the number of the node

    \returns number of CNs in the current cycle
    """
    if not (isinstance(nodeNumber, int)):   # numbers have to be integer
      return
    if nodeNumber < 0 or nodeNumber >= self.getNumNodes():                # and in correct size
      return
    if self._C_Cycle.getPacket(nodeNumber) == NULL:
      return
    return self._C_Cycle.getPacket(nodeNumber).getDestNode()

  def getPacketTime(self, nodeNumber):
    """
    \brief This method returns the device type of a node.

    \param nodeNumber the number of the node

    \returns number of CNs in the current cycle
    """
    if not (isinstance(nodeNumber, int)):   # numbers have to be integer
      return
    if nodeNumber < 0 or nodeNumber >= self.getNumNodes():                # and in correct size
      return
    if self._C_Cycle.getPacket(nodeNumber) == NULL:
      return
    return self._C_Cycle.getPacket(nodeNumber).getTime()

  def getPacketType(self, nodeNumber):
    """
    \brief This method returns the device type of a node.

    \param nodeNumber the number of the node

    \returns number of CNs in the current cycle
    """
    if not (isinstance(nodeNumber, int)):   # numbers have to be integer
      return
    if nodeNumber < 0 or nodeNumber >= self.getNumNodes():                # and in correct size
      return
    if self._C_Cycle.getPacket(nodeNumber) == NULL:
      return
    return self._C_Cycle.getPacket(nodeNumber).getTypeStr().decode()

# creating a cycle from Cycle*, internal function ...
cdef createCycle(CCycle.Cycle* curCycle):
  cyc = Cycle()
  cyc._C_Cycle = curCycle
  return cyc
