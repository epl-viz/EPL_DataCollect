import Filters
import Plugin
import PluginAPI as api

class Sample_Filter(Plugin.Plugin):
  def getID(self):
    return "Sample_Filter"

  def initialize(self):
    if not api.requestFilter(self, Filters.INCLUDING.value):
      return False
    api.addFilter(self, 0x6000)
    api.addFilter(self, 0x1006)
    return True

  def run(self):
    pass
