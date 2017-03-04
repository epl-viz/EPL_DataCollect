import Cycle
import Plugin
import PluginAPI as api
import PluginGUIAPI

class SimplePlugin(Plugin.Plugin):

  def run(self):
    cy = self.getCycle()
    if cy.getCycleNum() % 5 == 0:
      print("Cycle Number:", cy.getCycleNum())
      print("AmountOfCN:", cy.getAmountOfCN())

  def getID(self):
    return "SimplePlugin"
