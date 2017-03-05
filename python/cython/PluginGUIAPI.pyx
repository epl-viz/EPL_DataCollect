import Plugin
import Events

"""
\brief This class enables a plugin user to easily add GUI representation
specific events to highlight or start and stop the representations quickly.
It therefore provides convenience methods that add events to the backend.

:version: 1.0.0
"""

def startLive(plugin):
  """
  \brief An easy method to use to add the start recording event to the current
  cycle.
  It will just start the live representation of the recording, the recording will
  still go on in the background.

  \param plugin the plugin currently used, has to be given

  \returns whether successful or not
  """
  if isinstance(plugin, Plugin.Plugin):
    return plugin.addEvent(Events.EV_STARTCAP.value, "", "")  # empty string added for method
  return False

def stopLive(plugin):
  """
  \brief An easy method to use to add the stop recording event to the current
  cycle.
  It will just stop the live representation of the recording, the recording will
  still go on in the background.

  \param plugin the plugin currently used, has to be given

  \returns whether successful or not
  """
  if isinstance(plugin, Plugin.Plugin):
    return plugin.addEvent(Events.EV_ENDCAP.value, "", "")
  return False

def highlightMN(plugin):
  """
  \brief This method highlights the MN in the gui.

  \param plugin the plugin currently used, has to be given

  \returns a bool showing whether the node highlight is successful
  """
  if isinstance(plugin, Plugin.Plugin):
    return plugin.addEvent(Events.EV_HIGHLIGHT_MN.value, "", "")
  return False

def highlightNode(plugin, node):
  """
  \brief This method highlights a specific in the graphical representation.

  \param plugin the plugin currently used, has to be given
  \param Node node : The node to be highlighted in the graphical representation.

  \returns a bool showing whether the node highlight is successful
  """
  if isinstance(plugin, Plugin.Plugin) and isinstance(node, int):
    if node >= 0:
      return plugin.addEvent(Events.EV_HIGHLIGHT_CN.value, str(node), "")
    return False
  return False

def highlightODEntry(plugin, entry, level):
  """
  \brief Highlights a specific OD Entry in the graphical representations.
  The level of highlight is set by an unsigned integer, the graphical / other
  representation can build its own highlighting based on this number.

  \param plugin the plugin currently used, has to be given
  \param ODEntry entry : The entry to be highlighted.
  \param unsigned int level : The level of highlighting to be used in the graphical representations, a float in (0.0, 1.0]

  \returns true if highlighting was added successful.
  """
  if isinstance(plugin, Plugin.Plugin) and isinstance(entry, int) and isinstance(level, float):
    if 0.0 < level <= 1.0 and entry >= 0x0000 and entry <= 0xFFFF:
      return plugin.addEvent(Events.EV_HIGHLIGHT_OD_ENTRY.value, str(entry), str(level))
  return False
