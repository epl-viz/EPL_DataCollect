import Plugin
import Events
import Filters

class Test_PluginInternal(Plugin.Plugin):
  def getID(self):
    return "Test_PluginInternal"

  def initialize(self):
    if self.registerInt(2) or self.registerInt(None):
      return False
    if not self.registerInt("MyIntStorage"):
      return False
    if self.registerInt("MyIntStorage"):
      return False

    if self.registerStr(2) or self.registerStr(None):
      return False
    if not self.registerStr("MyStrStorage"):
      return False
    if self.registerStr("MyIntStorage") or self.registerStr("MyStrStorage"):
      return False
    return True

  def run(self):
    # getting cycle...
    cy = self.getCycle()
    if cy is None:
      print("ERROR cycle has not been retrieved", self)

    # storage cases
    if self.getStorage(None) is not None:
      print("ERROR a storage shouldnt have been retrieved", self)
    if self.getStorage("here") is None:
      print("ERROR a storage should have been retrieved", self)
    if self.setStorage(None, None) or self.setStorage(2, None):
      print("ERROR a storage should not have been set", self)
    if self.setStorage("AData", 2):
      print("ERROR a storage should not have been set", self)

    if cy.getCycleNum() == 50:
      self.setStorage("DataStore", "MyD")
    if cy.getCycleNum() == 100 and not (self.getStorage("DataStore") == "MyD"):
      print("ERROR data not set", self)

    # data cases
    if not self.setData("MyIntStorage", 2):
      print("ERROR data not set", self)

    if not int(self.getData("MyIntStorage")) == 2:
      print("ERROR setting data before", self)
    if not self.getData("abc") is "":
      print("ERROR retrieving data", self)
    if not self.getData(None) is None:
      print("ERROR should have none", self)

    # event adding
    if not self.addEvent(Events.EV_STARTCAP.value, "", ""):
      print("ERROR adding ev", self)
    if not self.addEvent(Events.EV_ENDCAP.value, "", ""):
      print("ERROR adding ev", self)
    if not self.addEvent(Events.EV_HIGHLIGHT_MN.value, "2", ""):
      print("ERROR adding ev", self)
    if not self.addEvent(Events.EV_HIGHLIGHT_CN.value, "2", ""):
      print("ERROR adding ev", self)
    if not self.addEvent(Events.EV_HIGHLIGHT_OD_ENTRY.value, "0x1000", "10"):
      print("ERROR adding ev", self)
    if not self.addEvent(Events.EV_JUMPTOTIME.value, "100", ""):
      print("ERROR adding ev", self)
    if not self.addEvent(Events.EV_TEXT.value, "This is a sample text", ""):
      print("ERROR adding ev", self)

    if self.addEvent(Events.EV_STARTCAP.value, None, ""):
      print("ERROR event should not be added", self)
    if self.addEvent(Events.EV_ENDCAP.value, None, None):
      print("ERROR event should not be added", self)
    if self.addEvent(Events.EV_HIGHLIGHT_MN.value, None, ""):
      print("ERROR event should not be added", self)
    if self.addEvent(Events.EV_HIGHLIGHT_CN.value, "a", ""):
      print("ERROR event should not be added", self)
    if self.addEvent(Events.EV_HIGHLIGHT_OD_ENTRY.value, "-1", "10"):
      print("ERROR event should not be added", self)
    if self.addEvent(Events.EV_JUMPTOTIME.value, "-1", ""):
      print("ERROR event should not be added", self)
    if self.addEvent(Events.EV_TEXT.value, None, ""):
      print("ERROR event should not be added", self)

    if self.addFilter(Filters.INCLUDING.value, None):
      print("ERROR filter should not have been added", self)
    if self.addFilter(None, "asdf"):
      print("ERROR filter should not have been added", self)
    if not self.addFilter(Filters.INCLUDING.value, "asdf"):
      print("ERROR filter should have been added", self)
