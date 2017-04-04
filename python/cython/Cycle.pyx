cimport CCycle
cimport CPlugin
from libcpp.vector cimport vector
from libcpp.string cimport string
from libcpp cimport bool
from libcpp.unordered_map cimport unordered_map

cdef class Cycle:
  """Class representing a cycle.

  :param _C_Cycle: Hidden cycle reference
  """
  cdef CCycle.Cycle* _C_Cycle

  def __cinit__(self):
    pass

  cpdef int getNumNodes(self):
    """Getting the amount of nodes.

    This method returns the amount of controlled nodes currently active. The amount of controlled nodes is retrieved from a specific cycle, thus representing the CNs active at that given cycle.

    :return: number of CNs in the current cycle
    :rtype: as integer

    .. seealso:: getAmountOfCN
    """
    return self._C_Cycle.getNumNodes()

  cpdef int getCycleNum(self):
    """Getting the current cycles number since start.

    The number ranges from 0 to uint32t_max and are unsigned.

    :return: number of the current cycle
    :rtype: as integer
    """
    return self._C_Cycle.getCycleNum()

  cdef void updateCycle(self, CCycle.Cycle* newCycle):
    # internal method
    if newCycle != NULL:
      self._C_Cycle = newCycle

  def getActiveEvents(self):
    """Getting all active events in the cycle.

    Getting the active events of the cycle.

    :return: touple of events
    :rtype: touple -> (type as int, string type as str, name of event, description, integer flag)

    .. note::
      Note that this is mostly empty and only contains data of python plugins created events.

    :Example:
      This method is used if you try to get old cycles and check for their events or have dependent other plugins that may fire events first.
    """
    listOfEvents = []
    cdef vector[CCycle.EventBase *] eventVector = self._C_Cycle.getActiveEvents()
    for event in eventVector:
      pyEvent = (<int>event.getType(), event.getTypeAsString().decode(), event.getName().decode(), event.getDescription().decode(), event.getEventFlags())
      listOfEvents.append(pyEvent)
    return listOfEvents

  def getNodeStatus(self, nodeNumber):
    """Returns the status of the node given

    Returning the status as a python str of any currently active node.

    :param nodeNumber: number of the node

    :return: representation of the state in the cycle
    :rtype: python str or None object if node not available

    .. note::
      All POWERLINK states are available
      { OFF, PRE_OPERATIONAL_1, PRE_OPERATIONAL_2, READY, OPERATIONAL, ... }

    :Example:
      comparing like 'cycle.getNodeStatus(1) == "READY"'
    """
    if not (isinstance(nodeNumber, int)):   # numbers have to be integer
      return
    if nodeNumber < 0:                # and in correct size
      return
    if self._C_Cycle.getNode(nodeNumber) == NULL:
      return
    return self._C_Cycle.getNode(nodeNumber).getStatusStr().decode()

  def getODEntry(self, nodeNumber, odNumber):
    """Returning ODEntry of a node.

    This method returns the ODEntry, subindex 0 represented as string. None if not available, standard values are 0 or ""<N/A>" on complex types

    :param nodeNumber: number of the node
    :param odNumber: number of the ODEntry
    :type nodeNumber: int
    :type odNumber: int


    :return: str representation of od entry
    :rtype: python str

    .. note::
      Check for correct types if you want to cast.
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
    """Getting complex ODEntry at given subindex.

    This method returns the ODEntry, subindex as string represented. None if not available, standard values are 0 or ""<N/A>" on complex types

    :param nodeNumber: number of the node
    :param odNumber: number of the ODEntry
    :param subindex: subindex number
    :type nodeNumber: int
    :type odNumber: int
    :type subindex: int

    :return: str representation of od entry
    :rtype: python str

    .. note::
      ODEntry with subindex, if non available returns 0th subindex
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
    """Getting the amount of nodes.

    This method returns the amount of controlled nodes currently active. The amount of controlled nodes is retrieved from a specific cycle, thus representing the CNs active at that given cycle.

    :return: number of CNs in the current cycle
    :rtype: as integer

    .. seealso:: getNumNodes
    """
    return self._C_Cycle.getNumNodes()

  ## getter methods for getting the commonly used node stuff, None if the node is not available !
  # str returned by:
  def getDeviceType(self, nodeNumber):
    """This method returns the device type of a node.

    :param nodeNumber: number of the node
    :type nodeNumber: int

    :return: device type of CNs in the current cycle as str
    :rtype: pythons str or None if node not available
    """
    if not (isinstance(nodeNumber, int)):   # numbers have to be integer
      return
    if nodeNumber < 0:                # and in correct size
      return
    if self._C_Cycle.getNode(nodeNumber) == NULL:
      return
    return self._C_Cycle.getNode(nodeNumber).getDeviceType().decode()

  def getIPAddress(self, nodeNumber):
    """This method returns the ip address of a node.

    :param nodeNumber: the number of the node
    :type nodeNumber: int

    :return: ip address of CNs in the current cycle as str
    :rtype: pythons str or None if node not available
    """
    if not (isinstance(nodeNumber, int)):   # numbers have to be integer
      return
    if nodeNumber < 0:                # and in correct size
      return
    if self._C_Cycle.getNode(nodeNumber) == NULL:
      return
    return self._C_Cycle.getNode(nodeNumber).getIPAddress().decode()

  def getSubnetMask(self, nodeNumber):
    """This method returns the subnet mask of a node.

    :param nodeNumber: the number of the node
    :type nodeNumber: int

    :return: subnet mask of CNs in the current cycle as str
    :rtype: pythons str or None if node not available
    """
    if not (isinstance(nodeNumber, int)):   # numbers have to be integer
      return
    if nodeNumber < 0:                # and in correct size
      return
    if self._C_Cycle.getNode(nodeNumber) == NULL:
      return
    return self._C_Cycle.getNode(nodeNumber).getSubnetMask().decode()

  def getDefaultGateway(self, nodeNumber):
    """This method returns the default gateway of a node.

    :param nodeNumber: the number of the node
    :type nodeNumber: int

    :return: default gateway of CNs in the current cycle as str
    :rtype: pythons str or None if node not available
    """
    if not (isinstance(nodeNumber, int)):   # numbers have to be integer
      return
    if nodeNumber < 0:                # and in correct size
      return
    if self._C_Cycle.getNode(nodeNumber) == NULL:
      return
    return self._C_Cycle.getNode(nodeNumber).getDefaultGateway().decode()

  def getHostName(self, nodeNumber):
    """This method returns the host name of a node.

    :param nodeNumber: the number of the node
    :type nodeNumber: int

    :return: host name of CNs in the current cycle as str
    :rtype: pythons str or None if node not available
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
    """This method returns the profile of a node.

    :param nodeNumber: the number of the node
    :type nodeNumber: int

    :return: profile of CNs in the current cycle as str
    :rtype: pythons int or None if node not available
    """
    if not (isinstance(nodeNumber, int)):   # numbers have to be integer
      return
    if nodeNumber < 0:                # and in correct size
      return
    if self._C_Cycle.getNode(nodeNumber) == NULL:
      return
    return self._C_Cycle.getNode(nodeNumber).getProfile()

  def getVendorId(self, nodeNumber):
    """This method returns the vendor id of a node.

    :param nodeNumber: the number of the node
    :type nodeNumber: int

    :return: vendor id of CNs in the current cycle as int
    :rtype: pythons int or None if node not available
    """
    if not (isinstance(nodeNumber, int)):   # numbers have to be integer
      return
    if nodeNumber < 0:                # and in correct size
      return
    if self._C_Cycle.getNode(nodeNumber) == NULL:
      return
    return self._C_Cycle.getNode(nodeNumber).getVendorId()

  def getProductCode(self, nodeNumber):
    """This method returns the product code of a node.

    :param nodeNumber: the number of the node
    :type nodeNumber: int

    :return: product code of CNs in the current cycle as int
    :rtype: pythons int or None if node not available
    """
    if not (isinstance(nodeNumber, int)):   # numbers have to be integer
      return
    if nodeNumber < 0:                # and in correct size
      return
    if self._C_Cycle.getNode(nodeNumber) == NULL:
      return
    return self._C_Cycle.getNode(nodeNumber).getProductCode()

  def getRevisionNumber(self, nodeNumber):
    """This method returns the revision number of a node.

    :param nodeNumber: the number of the node
    :type nodeNumber: int

    :return: revision number of CNs in the current cycle as int
    :rtype: pythons int or None if node not available
    """
    if not (isinstance(nodeNumber, int)):   # numbers have to be integer
      return
    if nodeNumber < 0:                # and in correct size
      return
    if self._C_Cycle.getNode(nodeNumber) == NULL:
      return
    return self._C_Cycle.getNode(nodeNumber).getRevisionNumber()

  def getSerialNumber(self, nodeNumber):
    """This method returns the serial number of a node.

    :param nodeNumber: the number of the node
    :type nodeNumber: int

    :return: serial number of CNs in the current cycle as int
    :rtype: pythons int or None if node not available
    """
    if not (isinstance(nodeNumber, int)):   # numbers have to be integer
      return
    if nodeNumber < 0:                # and in correct size
      return
    if self._C_Cycle.getNode(nodeNumber) == NULL:
      return
    return self._C_Cycle.getNode(nodeNumber).getSerialNumber()

  def getResponseTime(self, nodeNumber):
    """This method returns the response time of a node.

    :param nodeNumber: the number of the node
    :type nodeNumber: int

    :return: response time of CNs in the current cycle as int
    :rtype: pythons int or None if node not available
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
    """This method returns the device type of a node.

    :param nodeNumber: the number of the node
    :type nodeNumber: int

    :return: number of CNs in the current cycle
    :rtype: pythons int or None if packet not available
    """
    if not (isinstance(nodeNumber, int)):   # numbers have to be integer
      return
    if nodeNumber < 0:                # and in correct size
      return
    if self._C_Cycle.getPacket(nodeNumber) == NULL:
      return
    return self._C_Cycle.getPacket(nodeNumber).getSrcNode()

  def getPacketDest(self, nodeNumber):
    """This method returns the device type of a node.

    :param nodeNumber: the number of the node
    :type nodeNumber: int

    :return: number of CNs in the current cycle
    :rtype: pythons int or None if packet not available
    """
    if not (isinstance(nodeNumber, int)):   # numbers have to be integer
      return
    if nodeNumber < 0 or nodeNumber >= self.getNumNodes():                # and in correct size
      return
    if self._C_Cycle.getPacket(nodeNumber) == NULL:
      return
    return self._C_Cycle.getPacket(nodeNumber).getDestNode()

  def getPacketTime(self, nodeNumber):
    """This method returns the device type of a node.

    :param nodeNumber: the number of the node
    :type nodeNumber: int

    :return: number of CNs in the current cycle
    :rtype: pythons int or None if packet not available
    """
    if not (isinstance(nodeNumber, int)):   # numbers have to be integer
      return
    if nodeNumber < 0 or nodeNumber >= self.getNumNodes():                # and in correct size
      return
    if self._C_Cycle.getPacket(nodeNumber) == NULL:
      return
    return self._C_Cycle.getPacket(nodeNumber).getTime()

  def getPacketType(self, nodeNumber):
    """This method returns the device type of a node.

    :param nodeNumber: the number of the node
    :type nodeNumber: int

    :return: number of CNs in the current cycle
    :rtype: pythons int or None if packet not available
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
