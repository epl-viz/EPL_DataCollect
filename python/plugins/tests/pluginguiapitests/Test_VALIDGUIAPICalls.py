import Plugin
import PluginGUIAPI as gui
import unittest

PYTHON_PRE = "[Python]"
ERROR_MSG = PYTHON_PRE + "Python failed, if compiled with C++ this is an error"

class Test_VALIDGUIAPICalls(Plugin.Plugin, unittest.TestCase):
  def getID(self):
    return "Test_VALIDGUIAPICalls"

  def run(self):
    self.assertTrue(gui.highlightMN(self), PYTHON_PRE + "ERROR adding ev") # this will only create one event

    cy = self.getCycle()
    if cy.getCycleNum() % 2 == 0: # this will add each event once every 2 cycles
      self.assertTrue(gui.startLive(self), PYTHON_PRE + "ERROR adding ev")
      self.assertTrue(gui.stopLive(self), PYTHON_PRE + "ERROR adding ev")
      self.assertTrue(gui.highlightNode(self, 1), PYTHON_PRE + "ERROR adding ev")
      self.assertTrue(gui.highlightODEntry(self, 0x1000, 10), PYTHON_PRE + "ERROR adding ev")
