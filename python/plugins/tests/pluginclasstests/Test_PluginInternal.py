import Plugin
import Events
import Filters
import unittest

PYTHON_PRE = "[Python]"
ERROR_MSG = PYTHON_PRE + "Python failed, if compiled with C++ this is an error"

class Test_PluginInternal(Plugin.Plugin, unittest.TestCase):
  def getID(self):
    return "Test_PluginInternal"

  def initialize(self):
    self.assertFalse(self.registerInt(2))
    self.assertFalse(self.registerInt(None))

    self.assertTrue(self.registerInt("MyIntStorage"))
    self.assertFalse(self.registerInt("MyIntStorage"))

    self.assertFalse(self.registerStr(2))
    self.assertFalse(self.registerStr(None))

    self.assertTrue(self.registerStr("MyStrStorage"))
    self.assertFalse(self.registerStr("MyStrStorage"))
    self.assertFalse(self.registerStr("MyIntStorage"))
    return True

  def run(self):
    # getting cycle...
    cy = self.getCycle()
    if cy is None:
      print(PYTHON_PRE, "ERROR cycle has not been retrieved", self)
      self.fail(ERROR_MSG)

    # storage cases
    if self.getStorage(None) is not None:
      print(PYTHON_PRE, "ERROR a storage shouldnt have been retrieved", self)
      self.fail(ERROR_MSG)
    if self.getStorage("here") is None:
      print(PYTHON_PRE, "ERROR a storage should have been retrieved", self)
      self.fail(ERROR_MSG)
    if self.setStorage(None, None) or self.setStorage(2, None):
      print(PYTHON_PRE, "ERROR a storage should not have been set", self)
      self.fail(ERROR_MSG)
    if self.setStorage("AData", 2):
      print(PYTHON_PRE, "ERROR a storage should not have been set", self)
      self.fail(ERROR_MSG)

    if cy.getCycleNum() == 50:
      self.setStorage("DataStore", "MyD")
    if cy.getCycleNum() == 100 and not (self.getStorage("DataStore") == "MyD"):
      print(PYTHON_PRE, "ERROR data not set", self)
      self.fail(ERROR_MSG)

    # data cases
    if not self.setData("MyIntStorage", 2):
      print(PYTHON_PRE, "ERROR data not set", self)
      self.fail(ERROR_MSG)

    if not int(self.getData("MyIntStorage")) == 2:
      print(PYTHON_PRE, "ERROR setting data before", self)
      self.fail(ERROR_MSG)
    if not self.getData("abc") is "":
      print(PYTHON_PRE, "ERROR retrieving data", self)
      self.fail(ERROR_MSG)
    if not self.getData(None) is None:
      print(PYTHON_PRE, "ERROR should have none", self)
      self.fail(ERROR_MSG)

    # event adding
    if not self.addEvent(Events.EV_STARTCAP.value, "", ""):
      print(PYTHON_PRE, "ERROR adding ev", self)
      self.fail(ERROR_MSG)
    if not self.addEvent(Events.EV_ENDCAP.value, "", ""):
      print(PYTHON_PRE, "ERROR adding ev", self)
      self.fail(ERROR_MSG)
    if not self.addEvent(Events.EV_HIGHLIGHT_MN.value, "2", ""):
      print(PYTHON_PRE, "ERROR adding ev", self)
      self.fail(ERROR_MSG)
    if not self.addEvent(Events.EV_HIGHLIGHT_CN.value, "2", ""):
      print(PYTHON_PRE, "ERROR adding ev", self)
      self.fail(ERROR_MSG)
    if not self.addEvent(Events.EV_HIGHLIGHT_OD_ENTRY.value, "0x1000", "10"):
      print(PYTHON_PRE, "ERROR adding ev", self)
      self.fail(ERROR_MSG)
    if not self.addEvent(Events.EV_JUMPTOTIME.value, "100", ""):
      print(PYTHON_PRE, "ERROR adding ev", self)
      self.fail(ERROR_MSG)
    if not self.addEvent(Events.EV_TEXT.value, "This is a sample text", ""):
      print(PYTHON_PRE, "ERROR adding ev", self)
      self.fail(ERROR_MSG)

    if self.addEvent(Events.EV_STARTCAP.value, None, ""):
      print(PYTHON_PRE, "ERROR event should not be added", self)
      self.fail(ERROR_MSG)
    if self.addEvent(Events.EV_ENDCAP.value, None, None):
      print(PYTHON_PRE, "ERROR event should not be added", self)
      self.fail(ERROR_MSG)
    if self.addEvent(Events.EV_HIGHLIGHT_MN.value, None, ""):
      print(PYTHON_PRE, "ERROR event should not be added", self)
      self.fail(ERROR_MSG)
    if self.addEvent(Events.EV_HIGHLIGHT_CN.value, "a", ""):
      print(PYTHON_PRE, "ERROR event should not be added", self)
      self.fail(ERROR_MSG)
    if self.addEvent(Events.EV_HIGHLIGHT_OD_ENTRY.value, "-1", "10"):
      print(PYTHON_PRE, "ERROR event should not be added", self)
      self.fail(ERROR_MSG)
    if self.addEvent(Events.EV_JUMPTOTIME.value, "-1", ""):
      print(PYTHON_PRE, "ERROR event should not be added", self)
      self.fail(ERROR_MSG)
    if self.addEvent(Events.EV_TEXT.value, None, ""):
      print(PYTHON_PRE, "ERROR event should not be added", self)
      self.fail(ERROR_MSG)

    if self.addFilter(Filters.INCLUDING.value, None):
      print(PYTHON_PRE, "ERROR filter should not have been added", self)
      self.fail(ERROR_MSG)
    if self.addFilter(None, "asdf"):
      print(PYTHON_PRE, "ERROR filter should not have been added", self)
      self.fail(ERROR_MSG)
    if (not self.addFilter(Filters.INCLUDING.value, "asdf")) and cy.getCycleNum() != 0:
      print(PYTHON_PRE, "ERROR filter should have been added", self)
      self.fail(ERROR_MSG)
