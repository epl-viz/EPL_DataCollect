cimport CCycle
cimport CPlugin
from libcpp.string cimport string
from libcpp.vector cimport vector
"""
\brief The plugin api provides some basic functionality and convenience method
for user plugins.
It interacts with the backend and modifies / adds events and additional user
data.

:version: 0.0.1
:author: Denis Megerle
"""

def addFilter(filters):
  """
  \brief This method adds a filter for specific ODs or other data.

  \param filters : A list of strings to be shown in the graphical user interface.

  \return whether the filter addition has been successful
  """

  ## TODO: implement ## -> just create stdstring vector in PythonPlugin and create addFilter method in Pythonplugin, add filter "asdf asfdff asdffe",
  #Trenner sind " "
  pass
