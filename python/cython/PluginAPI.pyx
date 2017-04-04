import Plugin
from libcpp.string cimport string
from libcpp.vector cimport vector

"""
The plugin api provides some basic functionality and convenience method
for user plugins.
It interacts with the backend and modifies / adds filters and additional user
data.
"""
def addFilter(plugin, filter):
  """Adding filter.

  This method adds a filter for specific ODs or other data.

  :param filter: Ints to be filtered
  :type filter: python int
  """
  if isinstance(plugin, Plugin.Plugin):
    plugin.addFilterEntry(filter)

def requestFilter(plugin, filterEnu):
  """Requesting a filter of given type

  Adding a filter to a plugin.

  :param filterEnu: filter type as int
  :type filterEnu: python int from Filters.<FILTER>.value

  .. note::
    A filter has to be added to each plugin if you want to add filter ODs.
  """
  if isinstance(plugin, Plugin.Plugin) and isinstance(filterEnu, int):
    return plugin.requestFilter(filterEnu)
  return False
