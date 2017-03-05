import Plugin
import PluginGUIAPI as gui

class Test_VALIDGUIAPICalls(Plugin.Plugin):
  def getID(self):
    return "Test_VALIDGUIAPICalls"

  def run(self):
    if not gui.highlightMN(self): # this will only create one event
      print("ERROR ADDING EV", self)
    cy = self.getCycle()
    if cy.getCycleNum() % 2 == 0: # this will add each event once every 2 cycles
      if not gui.startLive(self):
        print("ERROR ADDING EV", self)
      if not gui.stopLive(self):
        print("ERROR ADDING EV", self)
      if not gui.highlightNode(self, 1):
        print("ERROR ADDING EV", self)
      if not gui.highlightODEntry(self, 0x1000, 10):
        print("ERROR ADDING EV", self)
