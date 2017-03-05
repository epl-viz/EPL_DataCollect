import Plugin
import PluginGUIAPI as gui

class Test_INVALIDGUIAPICalls(Plugin.Plugin):
  def getID(self):
    return "Test_INVALIDGUIAPICalls"

  def run(self):
    cy = self.getCycle()
    if gui.startLive(None):
      print("ERROR, SHOULD NOT RETURN TRUE", self)
    if gui.startLive("this is not good"):
      print("ERROR, SHOULD NOT RETURN TRUE", self)
    if gui.stopLive(None):
      print("ERROR, SHOULD NOT RETURN TRUE", self)
    if gui.stopLive("this is not good"):
      print("ERROR, SHOULD NOT RETURN TRUE", self)
    if gui.highlightMN(None):
      print("ERROR, SHOULD NOT RETURN TRUE", self)
    if gui.highlightMN("this is not good"):
      print("ERROR, SHOULD NOT RETURN TRUE", self)
    if gui.highlightNode(None, None):
      print("ERROR, SHOULD NOT RETURN TRUE", self)
    if gui.highlightNode(None, "this is not good"):
      print("ERROR, SHOULD NOT RETURN TRUE", self)
    if gui.highlightNode("this is not good", None):
      print("ERROR, SHOULD NOT RETURN TRUE", self)
    if gui.highlightNode(None, 2):
      print("ERROR, SHOULD NOT RETURN TRUE", self)
    if gui.highlightNode(self, -1):
      print("ERROR, SHOULD NOT RETURN TRUE", self)
    if gui.highlightNode(self, 99999999):
      print("ERROR, SHOULD NOT RETURN TRUE", self)
    if gui.highlightODEntry(None, None, None):
      print("ERROR, SHOULD NOT RETURN TRUE", self)
    if gui.highlightODEntry(None, "asdf", None):
      print("ERROR, SHOULD NOT RETURN TRUE", self)
    if gui.highlightODEntry(self, None, None):
      print("ERROR, SHOULD NOT RETURN TRUE", self)
    if gui.highlightODEntry(self, 0x10000, 50):
      print("ERROR, SHOULD NOT RETURN TRUE", self)
