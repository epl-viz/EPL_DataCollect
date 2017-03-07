import Plugin
import Events
import Filters

class Sample_Functions(Plugin.Plugin):
  def getID(self):
    return "Sample_Functions"

  def initialize(self):
    if not self.registerInt("MyIntStorage"):  # register needed variables here (these are available for all plugins)
      return False
    if not self.registerStr("MyStrStorage"):  # same, return False if the register was unsuccessful so we know that the plugin could not be inited
      return False
    return True                               # have to return True if init was ok from this side

  def run(self):
    cy = self.getCycle()                      # here we get the current cycle each time

    print("Node 1 has status:", cy.getNodeStatus(1))

    if cy.getCycleNum() == 150:               # at the 150th cycle do ...
      # cycle functions
      print("Cycle of current stuff:", cy.getCycleNum())
      print("Number of Nodes active:", cy.getNumNodes(), "same as", cy.getAmountOfCN())  # Some functions...
      print("Node 1 has status:", cy.getNodeStatus(1))
      print("Node 1 has written in OD Entry 0x1006", cy.getODEntry(1, 0x1006))  # will return None if node not found, or standard value 0 if not yet used

      # events - through self.addEvent, always provide valid strings, at least empty ones
      print("Adding Event:", self.addEvent(Events.EV_STARTCAP.value, "", ""))
      print("Adding Event:", self.addEvent(Events.EV_ENDCAP.value, "", ""))
      print("Adding Event:", self.addEvent(Events.EV_HIGHLIGHT_MN.value, "", ""))
      print("Adding Event:", self.addEvent(Events.EV_HIGHLIGHT_CN.value, "1", ""))  # provide the cn as str in the first argument
      print("Adding Event:", self.addEvent(Events.EV_HIGHLIGHT_OD_ENTRY.value, "0x1500", "20")) # provide the od entry as str in first argument,
                                                                                              # level of highlighting as str in second, between 0 and 100
      print("Adding Event:", self.addEvent(Events.EV_JUMPTOTIME.value, "100", ""))
      print("Adding Event:", self.addEvent(Events.EV_TEXT.value, "some text to be added...", "")) # provide text as str in first argument

    print("in cycle", cy.getCycleNum(), "od 0x6200 sub 1 is", cy.getODEntry_Sub(1, 0x6200, 1))   # provide node, entry and subindex

    if cy.getCycleNum() == 200:               # 50 cycles later what events have been thrown at cycle 150 ?
      print(self.getCycleByNum(150).getActiveEvents())                                                   # now printing out the events active

