import Plugin

class Test_CycleAccess(Plugin.Plugin):
  def getID(self):
    return "Test_CycleAccess"

  def run(self):
    cy = self.getCycle()
    # node addition in cycle 64 !
    if cy.getAmountOfCN() != 1 and cy.getCycleNum() < 64:
      print("ERROR invalid amount of CNs", self)
    if cy.getAmountOfCN() != 2 and cy.getCycleNum() > 63:
      print("ERROR invalid amount of CNs", self)

    # changes in state should be detected at given cycles
    if cy.getNodeStatus(1) is not None and cy.getCycleNum() < 64:
      print("ERROR invalid node status", self)
    if cy.getNodeStatus(1) != "OFF" and 63 < cy.getCycleNum() < 66:
      print("ERROR invalid node status", self)
    if cy.getNodeStatus(1) != "PRE_OPERATIONAL_2" and 65 < cy.getCycleNum() < 95:
      print("ERROR invalid node status", self)
    if cy.getNodeStatus(1) != "READY_TO_OPERATE" and 94 < cy.getCycleNum() < 102:
      print("ERROR invalid node status", self)
    if cy.getNodeStatus(1) != "OPERATIONAL" and 101 < cy.getCycleNum():
      print("ERROR invalid node status", self)

    # changes in od entry 0x1006 (written)
    if cy.getODEntry(1, 0x1006) is not None and cy.getCycleNum() < 64:
      print("ERROR invalid od entry")
    if cy.getODEntry(1, 0x1006) != "100000" and cy.getCycleNum() > 78:  # set val to 100000 in cycle 79
      print("ERROR invalid od entry")
