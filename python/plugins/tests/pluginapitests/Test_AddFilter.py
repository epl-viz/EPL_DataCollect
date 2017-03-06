import Plugin
import Filters
import PluginGUIAPI as agui


class Test_AddFilter(Plugin.Plugin):
  def getID(self):
    return "Test_AddFilter"

  def run(self):
    if agui.addFilter(self, None, None):
      print("ERROR filter should not have been added", self)
    if agui.addFilter(self, None, "None"):
      print("ERROR filter should not have been added", self)
    if agui.addFilter(self, "None", None):
      print("ERROR filter should not have been added", self)
    if agui.addFilter(self, Filters.INCLUDING.value, None):
      print("ERROR filter should not have been added", self)
    if agui.addFilter(self, Filters.INCLUDING.value, 2):
      print("ERROR filter should not have been added", self)
    if agui.addFilter(self, 99999999, "asdf"):
      print("ERROR filter should not have been added", self)

    if not agui.addFilter(self, Filters.INCLUDING.value, "correct"):
      print("ERROR filter should have been added", self)
