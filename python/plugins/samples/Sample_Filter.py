import Filters
import Plugin
import PluginAPI as api

class Sample_Filter(Plugin.Plugin):
  def getID(self):
    return "Sample_Filter"

  def initialize(self):
    if not api.requestFilter(self, Filters.INCLUDING.value):    # adding a filter requires one to first request the filter based on type (see Filters)
      return False
    api.addFilter(self, 0x6000)                                 # then one can add entries to be filtered
    api.addFilter(self, 0x1006)
    return True

  def run(self):
    pass
