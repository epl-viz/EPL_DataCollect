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
    if gui.startLive(None):
      print(PYTHON_PRE, "ERROR, should not add ev", self)
      self.fail(ERROR_MSG)
    if gui.startLive("this is not good"):
      print(PYTHON_PRE, "ERROR, should not add ev", self)
      self.fail(ERROR_MSG)
    if gui.stopLive(None):
      print(PYTHON_PRE, "ERROR, should not add ev", self)
      self.fail(ERROR_MSG)
    if gui.stopLive("this is not good"):
      print(PYTHON_PRE, "ERROR, should not add ev", self)
      self.fail(ERROR_MSG)
    if gui.highlightMN(None):
      print(PYTHON_PRE, "ERROR, should not add ev", self)
      self.fail(ERROR_MSG)
    if gui.highlightMN("this is not good"):
      print(PYTHON_PRE, "ERROR, should not add ev", self)
      self.fail(ERROR_MSG)
    if gui.highlightNode(None, None):
      print(PYTHON_PRE, "ERROR, should not add ev", self)
      self.fail(ERROR_MSG)
    if gui.highlightNode(None, "this is not good"):
      print(PYTHON_PRE, "ERROR, should not add ev", self)
      self.fail(ERROR_MSG)
    if gui.highlightNode("this is not good", None):
      print(PYTHON_PRE, "ERROR, should not add ev", self)
      self.fail(ERROR_MSG)
    if gui.highlightNode(None, 2):
      print(PYTHON_PRE, "ERROR, should not add ev", self)
      self.fail(ERROR_MSG)
    if gui.highlightNode(self, -1):
      print(PYTHON_PRE, "ERROR, should not add ev", self)
      self.fail(ERROR_MSG)
    if gui.highlightNode(self, 99999999):
      print(PYTHON_PRE, "ERROR, should not add ev", self)
      self.fail(ERROR_MSG)
    if gui.highlightODEntry(None, None, None):
      print(PYTHON_PRE, "ERROR, should not add ev", self)
      self.fail(ERROR_MSG)
    if gui.highlightODEntry(None, "asdf", None):
      print(PYTHON_PRE, "ERROR, should not add ev", self)
      self.fail(ERROR_MSG)
    if gui.highlightODEntry(self, None, None):
      print(PYTHON_PRE, "ERROR, should not add ev", self)
      self.fail(ERROR_MSG)
    if gui.highlightODEntry(self, 0x10000, 50):
      print(PYTHON_PRE, "ERROR, should not add ev", self)
      self.fail(ERROR_MSG)
