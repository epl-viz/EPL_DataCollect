import Cycle
import Plugin
import PluginAPI as api
import PluginGUIAPI as agui
import Events

class SimplePlugin(Plugin.Plugin):

  def initialize(self):
    self.registerInt("Counter")
    return True

  def run(self):
    cy = self.getCycle()
    if cy.getCycleNum() % 5 == 0:
      print("Cycle Number:", cy.getCycleNum())
      print("AmountOfCN:", cy.getAmountOfCN())
      print("Evt added:", self.addEvent(0, "0", ""))
      print("Data:", self.getData("Counter"))
      data = int(self.getData("Counter"))
      self.setData("Counter", data + 1)
      print("Evt with enum added:", self.addEvent(Events.EV_STARTCAP.value, "0", ""))
      agui.highlightMN(self)

    if cy.getCycleNum() == 100:
      self.unload()

  def getID(self):
    return "SimplePlugin"
