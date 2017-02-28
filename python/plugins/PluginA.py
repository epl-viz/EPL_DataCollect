import Cycle 
import Plugin
import PluginAPI as api

class PluginA(Plugin.Plugin): 
 
  def initialize(self): 
    print("hey")
 
  def run(self): 
    print("PluginA running!!") 
    cy = self.getCycle() 
    print("amount of CN!!!:", cy.getAmountOfCN())	
    print("NODE STATUS:", cy.getNodeStatus(23))
    print("GET OD ENTRY:", cy.getODEntry(1, "234432"))
    print("Number of Nodes currently active", cy.getNumNodes()) 
    print(self.addEvent(0, "0")) 
    print(cy.getActiveEvents())
    self.registerInt("MYSTORAGE") 
    print(self.getStorage("WER"))
    print(self.setStorage("asdf", "bfdd"))
    print(self.getData("asdffff"))
    print("setting data str", self.setData("asdf", "asdf"))
    print("setting data int", self.setData("ffff", 23))
    print("setting data bool", self.setData("ffxx", False))
    print("setting random shit", self.setData("xxff", cy))
 
  def getID(self): 
    return "PluginA" 
 
  def getDependencies(self): 
    return "PluginA Deps" 
