cimport CCycle
cimport CPlugin
from libcpp.unordered_map cimport unordered_map
from libcpp.string cimport string
"""
\brief The plugin api provides some basic functionality and convenience method
for user plugins.
It interacts with the backend and modifies / adds events and additional user
data.

:version: 0.0.1
:author: Denis Megerle
"""

def getCycle(number):
  """
  \brief This method gets a cycle at any given moment. The capture starts at 0, so a plugin can technically access any cycle (and therefore also compare cycles). However this method should be called very rarely since it recalculates the cycle based on snapshots.

      \returns the 'number'th captured cycle
  """
  ## TODO: implement ##
  pass

def addFilter(filters):
  """
  \brief This method adds a filter for specific ODs or other data.

  \param filters : A list of strings to be shown in the graphical user interface.

  \return whether the filter addition has been successful
  """

  ## TODO: implement ##
  pass
