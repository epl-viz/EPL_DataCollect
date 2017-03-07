import Plugin
import PluginGUIAPI as gui
import unittest

PYTHON_PRE = "[Python]"
ERROR_MSG = PYTHON_PRE + "Python failed, if compiled with C++ this is an error"

class Test_INVALIDGUIAPICalls(Plugin.Plugin, unittest.TestCase):
  def getID(self):
    return "Test_INVALIDGUIAPICalls"

  def run(self):
    cy = self.getCycle()
    self.assertFalse(gui.startLive(None), PYTHON_PRE + "ERROR, should not add ev")
    self.assertFalse(gui.startLive("this is not good"), PYTHON_PRE + "ERROR, should not add ev")
    self.assertFalse(gui.stopLive(None), PYTHON_PRE + "ERROR, should not add ev")
    self.assertFalse(gui.stopLive("this is not good"), PYTHON_PRE + "ERROR, should not add ev")
    self.assertFalse(gui.highlightMN(None), PYTHON_PRE + "ERROR, should not add ev")
    self.assertFalse(gui.highlightMN("this is not good"), PYTHON_PRE + "ERROR, should not add ev")
    self.assertFalse(gui.highlightNode(None, None), PYTHON_PRE + "ERROR, should not add ev")
    self.assertFalse(gui.highlightNode(None, "this is not good"), PYTHON_PRE + "ERROR, should not add ev")
    self.assertFalse(gui.highlightNode("this is not good", None), PYTHON_PRE + "ERROR, should not add ev")
    self.assertFalse(gui.highlightNode(None, 2), PYTHON_PRE + "ERROR, should not add ev")
    self.assertFalse(gui.highlightNode(self, -1), PYTHON_PRE + "ERROR, should not add ev")
    self.assertFalse(gui.highlightNode(self, 99999999), PYTHON_PRE + "ERROR, should not add ev")
    self.assertFalse(gui.highlightODEntry(None, None, None), PYTHON_PRE + "ERROR, should not add ev")
    self.assertFalse(gui.highlightODEntry(None, "asdf", None), PYTHON_PRE + "ERROR, should not add ev")
    self.assertFalse(gui.highlightODEntry(self, None, None), PYTHON_PRE + "ERROR, should not add ev")
    self.assertFalse(gui.highlightODEntry(self, 0x10000, 50), PYTHON_PRE + "ERROR, should not add ev")
