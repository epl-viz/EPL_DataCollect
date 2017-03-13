import Plugin
import Events
import PluginGUIAPI as gui

########
# highlighting a node on status change
########

class Sample_EventOnStatusChange(Plugin.Plugin):
  def getID(self):
    return "Sample_EventOnStatusChange"

  def initialize(self):
    return self.registerStr("FormerStatus")  # regging a string to save our status

  def run(self):
    cy = self.getCycle()
    if cy.getNodeStatus(1) is not None: # handling if node 1 is not in network ( or not anymore )
      if self.getData("FormerStatus") != cy.getNodeStatus(1):
        gui.highlightNode(self, 1)                                   # highlight node event
        self.setData("FormerStatus", cy.getNodeStatus(1))      # saving status in data
