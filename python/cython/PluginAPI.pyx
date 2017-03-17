import Plugin
from libcpp.string cimport string
from libcpp.vector cimport vector
"""
\brief The plugin api provides some basic functionality and convenience method
for user plugins.
It interacts with the backend and modifies / adds events and additional user
data.

:version: 1.0.0
"""

def addFilter(plugin, filter):
  """
  \brief This method adds a filter for specific ODs or other data.

  \param filters : Ints to be filtered
  """
  if isinstance(plugin, Plugin.Plugin):
    plugin.addFilterEntry(filter)

def requestFilter(plugin, filterEnu):
  if isinstance(plugin, Plugin.Plugin) and isinstance(filterEnu, int):
    return plugin.requestFilter(filterEnu)
  return False
