import Cycle
import Plugin
import PluginAPI as api
import PluginGUIAPI
import Events

class PluginA(Plugin.Plugin):

  def initialize(self):
    print("INITING PLUGIN", self.getID())
    print("REG INT", self.registerInt("MyIntStorage"))
    print("REG STR", self.registerStr("MyStrStorage"))
    return True

  def run(self):
    print("RUNNING PLUGIN", self.getID())
    # cycle stuff
    cy = self.getCycle()
    print("amount of CN!!!:", cy.getAmountOfCN())
    print("NODE STATUS:", cy.getNodeStatus(23))
    #print("GET OD ENTRY:", cy.getODEntry(1, "234432"))
    print("Number of Nodes currently active", cy.getNumNodes())


    # storage stuff
    print("SETTING STORAGE asdf", self.setStorage("asdf", "bfdd"))
    print("GETTING STORAGE asdf", self.getStorage("asdf"))

    # data stuff
    print(self.getData("MyIntStorage"), "<-- MyIntStorage before")
    print(self.getData("MyStrStorage"), "<-- MyStrStorage before")

    print("setting data int", self.setData("MyIntStorage", 2))
    print("setting data str", self.setData("MyStrStorage", "a setted data"))

    print(self.getData("MyIntStorage"), "<-- MyIntStorage after")
    print(self.getData("MyStrStorage"), "<-- MyStrStorage after")

    # event stuff
    print(cy.getActiveEvents())
    print("ADDING AN EVENT !!! :", self.addEvent(Events.EV_HIGHLIGHT_OD_ENTRY.value, "str", ""))
    print(cy.getActiveEvents())


    #print("startlive!!!", PluginGUIAPI.startLive(self))
    #print("higlightNode!!!", PluginGUIAPI.highlightNode(self, 2))
    #print("higlightODEntry!!!", PluginGUIAPI.highlightODEntry(self, 2, 1.0))
    cy2 = self.getCycleByNum(3)
    print("cy props", cy, "| cy2 props", cy2)
    print("RUNNING END PLUGIN", self.getID())

  def getID(self):
    return "PluginA"

  def getDependencies(self):
    return "PluginA Deps"
