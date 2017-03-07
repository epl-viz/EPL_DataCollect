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

