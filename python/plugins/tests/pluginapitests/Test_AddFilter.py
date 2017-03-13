import Plugin
import Filters
import PluginAPI as api
import unittest

PYTHON_PRE = "[Python]"
ERROR_MSG = PYTHON_PRE + "Python failed, if compiled with C++ this is an error"

class Test_AddFilter(Plugin.Plugin, unittest.TestCase):
  def getID(self):
    return "Test_AddFilter"

  def run(self):
    self.assertFalse(api.addFilter(self, None, None), PYTHON_PRE + "ERROR filter should not have been added")
    self.assertFalse(api.addFilter(self, None, "None"), PYTHON_PRE + "ERROR filter should not have been added")
    self.assertFalse(api.addFilter(self, "None", None), PYTHON_PRE + "ERROR filter should not have been added")
    self.assertFalse(api.addFilter(self, Filters.INCLUDING.value, None), PYTHON_PRE + "ERROR filter should not have been added")
    self.assertFalse(api.addFilter(self, Filters.INCLUDING.value, 2), PYTHON_PRE + "ERROR filter should not have been added")
    self.assertFalse(api.addFilter(self, 9999, 2), PYTHON_PRE + "ERROR filter should not have been added")

    if self.getCycle().getCycleNum() == 42:
      self.assertTrue(api.addFilter(self, Filters.INCLUDING.value, "correct"), PYTHON_PRE + "ERROR filter should have been added")
      self.assertTrue(api.addFilter(self, Filters.EXCLUDING.value, "stillCorrect"), PYTHON_PRE + "ERROR filter should have been added")
