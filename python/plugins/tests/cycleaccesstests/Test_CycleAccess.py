import Plugin
import unittest
import Events

PYTHON_PRE = "[Python]"
ERROR_MSG = PYTHON_PRE + "Python failed, if compiled with C++ this is an error"

# run test case after plugin test case
class Test_CycleAccess(Plugin.Plugin, unittest.TestCase):
  def getID(self):
    return "Test_CycleAccess"

  def run(self):
    cy = self.getCycle()

    # node addition in cycle 64 !
    if cy.getCycleNum() < 64:
      self.assertEqual(cy.getAmountOfCN(), 1, PYTHON_PRE + "ERROR invalid amount of CNs")
    if cy.getCycleNum() > 63:
      self.assertEqual(cy.getAmountOfCN(), 2, PYTHON_PRE + "ERROR invalid amount of CNs")

    # changes in state should be detected at given cycles
    if cy.getCycleNum() < 64:
      self.assertEqual(cy.getNodeStatus(1), None, PYTHON_PRE + "ERROR invalid node status")
    if 63 < cy.getCycleNum() < 66:
      self.assertEqual(cy.getNodeStatus(1), "OFF", PYTHON_PRE + "ERROR invalid node status")
    if 65 < cy.getCycleNum() < 95:
      self.assertEqual(cy.getNodeStatus(1), "PRE_OPERATIONAL_2", PYTHON_PRE + "ERROR invalid node status")
    if 94 < cy.getCycleNum() < 102:
      self.assertEqual(cy.getNodeStatus(1), "READY_TO_OPERATE", PYTHON_PRE + "ERROR invalid node status")
    if 101 < cy.getCycleNum():
      self.assertEqual(cy.getNodeStatus(1), "OPERATIONAL", PYTHON_PRE + "ERROR invalid node status")

    # changes in od entry 0x1006 (written)
    if cy.getCycleNum() < 64:
      self.assertEqual(cy.getODEntry(1, 0x1006), None, PYTHON_PRE + "ERROR invalid od entry")
    if cy.getCycleNum() > 78: # set val to 100000 in cycle 79
      self.assertEqual(cy.getODEntry(1, 0x1006), "100000", PYTHON_PRE + "ERROR invalid od entry")

    if cy.getCycleNum() == 100:
      self.assertTrue(self.addEvent(Events.EV_STARTCAP, "", ""))

    if cy.getCycleNum() == 150:
      self.assertEqual(len(self.getCycleByNum(100).getActiveEvents()), 1, PYTHON_PRE + "ERROR amount of events")

    # metadata methods
    self.assertEqual(cy.getDeviceType(-1), None, PYTHON_PRE + "ERROR should be None")
    self.assertEqual(cy.getDeviceType(99999), None, PYTHON_PRE + "ERROR should be None")
    self.assertEqual(cy.getIPAddress(-1), None, PYTHON_PRE + "ERROR should be None")
    self.assertEqual(cy.getIPAddress(99999), None, PYTHON_PRE + "ERROR should be None")
    self.assertEqual(cy.getSubnetMask(-1), None, PYTHON_PRE + "ERROR should be None")
    self.assertEqual(cy.getSubnetMask(99999), None, PYTHON_PRE + "ERROR should be None")
    self.assertEqual(cy.getDefaultGateway(-1), None, PYTHON_PRE + "ERROR should be None")
    self.assertEqual(cy.getDefaultGateway(99999), None, PYTHON_PRE + "ERROR should be None")
    self.assertEqual(cy.getHostName(-1), None, PYTHON_PRE + "ERROR should be None")
    self.assertEqual(cy.getHostName(99999), None, PYTHON_PRE + "ERROR should be None")
    self.assertEqual(cy.getProfile(-1), None, PYTHON_PRE + "ERROR should be None")
    self.assertEqual(cy.getProfile(99999), None, PYTHON_PRE + "ERROR should be None")
    self.assertEqual(cy.getVendorId(-1), None, PYTHON_PRE + "ERROR should be None")
    self.assertEqual(cy.getVendorId(99999), None, PYTHON_PRE + "ERROR should be None")
    self.assertEqual(cy.getProductCode(-1), None, PYTHON_PRE + "ERROR should be None")
    self.assertEqual(cy.getProductCode(99999), None, PYTHON_PRE + "ERROR should be None")
    self.assertEqual(cy.getRevisionNumber(-1), None, PYTHON_PRE + "ERROR should be None")
    self.assertEqual(cy.getRevisionNumber(99999), None, PYTHON_PRE + "ERROR should be None")
    self.assertEqual(cy.getSerialNumber(-1), None, PYTHON_PRE + "ERROR should be None")
    self.assertEqual(cy.getSerialNumber(99999), None, PYTHON_PRE + "ERROR should be None")
    self.assertEqual(cy.getResponseTime(-1), None, PYTHON_PRE + "ERROR should be None")
    self.assertEqual(cy.getResponseTime(99999), None, PYTHON_PRE + "ERROR should be None")

    if cy.getCycleNum() > 100:
      self.assertNotEqual(cy.getDeviceType(1), None, PYTHON_PRE + "ERROR should not be None")
      self.assertNotEqual(cy.getIPAddress(1), None, PYTHON_PRE + "ERROR should not be None")
      self.assertNotEqual(cy.getSubnetMask(1), None, PYTHON_PRE + "ERROR should not be None")
      self.assertNotEqual(cy.getDefaultGateway(1), None, PYTHON_PRE + "ERROR should not be None")
      self.assertNotEqual(cy.getHostName(1), None, PYTHON_PRE + "ERROR should not be None")
      self.assertNotEqual(cy.getProfile(1), None, PYTHON_PRE + "ERROR should not be None")
      self.assertNotEqual(cy.getVendorId(1), None, PYTHON_PRE + "ERROR should not be None")
      self.assertNotEqual(cy.getProductCode(1), None, PYTHON_PRE + "ERROR should not be None")
      self.assertNotEqual(cy.getRevisionNumber(1), None, PYTHON_PRE + "ERROR should not be None")
      self.assertNotEqual(cy.getSerialNumber(1), None, PYTHON_PRE + "ERROR should not be None")
      self.assertNotEqual(cy.getResponseTime(1), None, PYTHON_PRE + "ERROR should not be None")

    # packet methods
    self.assertEqual(cy.getPacketSrc(-1), None, PYTHON_PRE + "ERROR should be None")
    self.assertEqual(cy.getPacketSrc(99999), None, PYTHON_PRE + "ERROR should be None")
    self.assertEqual(cy.getPacketDest(-1), None, PYTHON_PRE + "ERROR should be None")
    self.assertEqual(cy.getPacketDest(99999), None, PYTHON_PRE + "ERROR should be None")
    self.assertEqual(cy.getPacketType(-1), None, PYTHON_PRE + "ERROR should be None")
    self.assertEqual(cy.getPacketType(99999), None, PYTHON_PRE + "ERROR should be None")
    self.assertEqual(cy.getPacketTime(-1), None, PYTHON_PRE + "ERROR should be None")
    self.assertEqual(cy.getPacketTime(99999), None, PYTHON_PRE + "ERROR should be None")

    if cy.getCycleNum() == 91:  # an async here
      self.assertEqual(cy.getPacketSrc(1), 1, PYTHON_PRE + "ERROR wrong return value")
      self.assertEqual(cy.getPacketDest(1), 255, PYTHON_PRE + "ERROR wrong return value")
      self.assertEqual(cy.getPacketType(1), "ASYNC_SEND", PYTHON_PRE + "ERROR wrong return value")
      self.assertEqual(cy.getPacketTime(1), 1486476688917882511, PYTHON_PRE + "ERROR wrong return value")

    if cy.getCycleNum() == 200: # random poll response
      self.assertEqual(cy.getPacketSrc(1), 1, PYTHON_PRE + "ERROR wrong return value")
      self.assertEqual(cy.getPacketDest(1), 255, PYTHON_PRE + "ERROR wrong return value")
      self.assertEqual(cy.getPacketType(1), "POLL_RESPONSE", PYTHON_PRE + "ERROR wrong return value")
      self.assertEqual(cy.getPacketTime(1), 1486476699817974623, PYTHON_PRE + "ERROR wrong return value")
