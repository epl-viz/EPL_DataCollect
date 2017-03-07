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
    if cy.getAmountOfCN() != 1 and cy.getCycleNum() < 64:
      print(PYTHON_PRE, "ERROR invalid amount of CNs", self)
      self.fail(ERROR_MSG)
    if cy.getAmountOfCN() != 2 and cy.getCycleNum() > 63:
      print(PYTHON_PRE, "ERROR invalid amount of CNs", self)
      self.fail(ERROR_MSG)

    # changes in state should be detected at given cycles
    if cy.getNodeStatus(1) is not None and cy.getCycleNum() < 64:
      print(PYTHON_PRE, "ERROR invalid node status", self)
      self.fail(ERROR_MSG)
    if cy.getNodeStatus(1) != "OFF" and 63 < cy.getCycleNum() < 66:
      print(PYTHON_PRE, "ERROR invalid node status", self)
      self.fail(ERROR_MSG)
    if cy.getNodeStatus(1) != "PRE_OPERATIONAL_2" and 65 < cy.getCycleNum() < 95:
      print(PYTHON_PRE, "ERROR invalid node status", self)
      self.fail(ERROR_MSG)
    if cy.getNodeStatus(1) != "READY_TO_OPERATE" and 94 < cy.getCycleNum() < 102:
      print(PYTHON_PRE, "ERROR invalid node status", self)
      self.fail(ERROR_MSG)
    if cy.getNodeStatus(1) != "OPERATIONAL" and 101 < cy.getCycleNum():
      print(PYTHON_PRE, "ERROR invalid node status", self)
      self.fail(ERROR_MSG)

    # changes in od entry 0x1006 (written)
    if cy.getODEntry(1, 0x1006) is not None and cy.getCycleNum() < 64:
      print(PYTHON_PRE, "ERROR invalid od entry")
      self.fail(ERROR_MSG)
    if cy.getODEntry(1, 0x1006) != "100000" and cy.getCycleNum() > 78:  # set val to 100000 in cycle 79
      print(PYTHON_PRE, "ERROR invalid od entry")
      self.fail(ERROR_MSG)

    if cy.getCycleNum() == 100:
      self.addEvent(Events.EV_STARTCAP, "", "")

    if cy.getCycleNum() == 150 and len(self.getCycleByNum(100).getActiveEvents()) != 1:
      print(PYTHON_PRE, "ERROR amount of events", self)
      self.fail(ERROR_MSG)
