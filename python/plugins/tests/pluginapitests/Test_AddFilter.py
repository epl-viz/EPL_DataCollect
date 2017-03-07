import Plugin
import Filters
import PluginGUIAPI as agui
import unittest

PYTHON_PRE = "[Python]"
ERROR_MSG = PYTHON_PRE + "Python failed, if compiled with C++ this is an error"

class Test_AddFilter(Plugin.Plugin, unittest.TestCase):
  def getID(self):
    return "Test_AddFilter"

  def run(self):
    if agui.addFilter(self, None, None):
      print(PYTHON_PRE, "ERROR filter should not have been added", self)
      self.fail(ERROR_MSG)
    if agui.addFilter(self, None, "None"):
      print(PYTHON_PRE, "ERROR filter should not have been added", self)
      self.fail(ERROR_MSG)
    if agui.addFilter(self, "None", None):
      print(PYTHON_PRE, "ERROR filter should not have been added", self)
      self.fail(ERROR_MSG)
    if agui.addFilter(self, Filters.INCLUDING.value, None):
      print(PYTHON_PRE, "ERROR filter should not have been added", self)
      self.fail(ERROR_MSG)
    if agui.addFilter(self, Filters.INCLUDING.value, 2):
      print(PYTHON_PRE, "ERROR filter should not have been added", self)
      self.fail(ERROR_MSG)
    if agui.addFilter(self, 99999999, "asdf"):
      print(PYTHON_PRE, "ERROR filter should not have been added", self)
      self.fail(ERROR_MSG)

    if not agui.addFilter(self, Filters.INCLUDING.value, "correct"):
      print(PYTHON_PRE, "ERROR filter should have been added", self)
      self.fail(ERROR_MSG)
