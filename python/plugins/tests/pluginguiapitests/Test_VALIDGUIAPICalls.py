import Plugin
import PluginGUIAPI as gui
import unittest

PYTHON_PRE = "[Python]"
ERROR_MSG = PYTHON_PRE + "Python failed, if compiled with C++ this is an error"

class Test_VALIDGUIAPICalls(Plugin.Plugin, unittest.TestCase):
  def getID(self):
    return "Test_VALIDGUIAPICalls"

  def run(self):
    if not gui.highlightMN(self): # this will only create one event
      print(PYTHON_PRE, "ERROR adding ev", self)
      self.fail(ERROR_MSG)
    cy = self.getCycle()
    if cy.getCycleNum() % 2 == 0: # this will add each event once every 2 cycles
      if not gui.startLive(self):
        print(PYTHON_PRE, "ERROR adding ev", self)
        self.fail(ERROR_MSG)
      if not gui.stopLive(self):
        print(PYTHON_PRE, "ERROR adding ev", self)
        self.fail(ERROR_MSG)
      if not gui.highlightNode(self, 1):
        print(PYTHON_PRE, "ERROR adding ev", self)
        self.fail(ERROR_MSG)
      if not gui.highlightODEntry(self, 0x1000, 10):
        print(PYTHON_PRE, "ERROR adding ev", self)
        self.fail(ERROR_MSG)
