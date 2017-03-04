import Cycle 
import Plugin
import PluginAPI as api
import PluginGUIAPI

class PluginA(Plugin.Plugin):
  def getID(self):
    return "PluginA"
  def initialize(self):
    return True
  def getDependencies(self):
    return "asdf"
