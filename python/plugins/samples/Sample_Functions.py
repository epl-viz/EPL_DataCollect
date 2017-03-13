import Plugin
import Events
import Filters

class Sample_Functions(Plugin.Plugin):
  def getID(self):
    return "Sample_Functions"

  def initialize(self):
    if not self.registerInt("sum"):  # register needed variables here (these are available for all plugins)
      return False
    if not self.registerStr("MyStrStorage"):  # same, return False if the register was unsuccessful so we know that the plugin could not be inited
      return False
    return True                               # have to return True if init was ok from this side

  def run(self):
    cy = self.getCycle()                      # here we get the current cycle each time

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

    if cy.getCycleNum() % 20 == 0:
      try:
        print("in cycle", cy.getCycleNum(), "od 0x6200 sub 1 is", cy.getODEntry_Sub(1, 0x6200, 1))   # provide node, entry and subindex
      except TypeError:
        pass
      print("also, profile is", cy.getProfile(1))

    if cy.getCycleNum() == 200:               # 50 cycles later what events have been thrown at cycle 150 ?
      print("Active Events at cycle 200")    # now printing out the events active
      for ev in self.getCycleByNum(150).getActiveEvents():
        print(ev)

    if cy.getCycleNum() % 50 == 0:
      print("in cycle", cy.getCycleNum(), "IP Adr of Node 1", cy.getIPAddress(1)) # lots of get methods can be used for nodes

    # use case : summing up over an entry. exception handling has to be used since the od entry might not be an int or just not available in every cycle
    i = 0
    try:
      i = int(cy.getODEntry_Sub(1, 0x6200, 1))
    except (TypeError, ValueError):
      pass  ## not an int here, meaning the odentry is either not inited or

    self.setData("sum", int(self.getData("sum")) + i)     # don't need to try catch data that has been registered as int !

    if cy.getCycleNum() == 200:
      print("Sum of 0x6200 up to cycle [", cy.getCycleNum(), "]", self.getData("sum"))

    if int(self.getData("sum")) >= 2000:  # as soon as the sum gets over 2000, this plugin will stop
      self.unload()


