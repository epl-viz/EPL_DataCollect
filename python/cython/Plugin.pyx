cimport CCycle
cimport CPlugin
import Cycle
import importlib
import sys
from libcpp.string cimport string

cdef class Plugin:
  """
  \brief This class represents a custom user plugin base.
  Users can inherit from this class and therefore create their own plugins.
  Plugins use the run method, which run each cycle allowing the user to check for
  data each cycle.

  \version 0.5.0
  \author Denis Megerle
  """

  def __cinit__(self):
    pass

  ###########################Template Methods#############################################
  cpdef initialize(self):
    """
    \brief The initialize method will only be called once, at the moment the plugin
    gets bound.
    It therefore is used to create datastructures / prepare the plugin for running
    at each cycle and for adding the ID and dependencies.
    By default this will return nothing, since not initializing a plugin is legit.

    \version 0.5.0
    \author Denis Megerle
    """
    pass  #initialize can be empty

  cpdef run(self):
    """
    \brief This method runs each cycle, here is the user plugins main.
    Users code will be executed each cycle thus this method should be kept quick. If
    this method runs to slow it will be stopped and rerun to prevent slowing down of
    the program.

    \version 0.5.0
    \author Denis Megerle
    """
    pass

  cpdef getDependencies(self):
    """
    \brief This method returns the dependencies of the plugin as a string.
    Dependencies are splitted by whitespace and the plugin will only be started if
    each dependency is resolved.

    \returns a string of dependencies

    \version 0.5.0
    \author Denis Megerle
    """
    return "" #Dependencies can be empty if needed

  cpdef getID(self):
    """
    \brief This method returns the unique ID of this plugin.

    \returns the string of the ID

    \version 0.5.0
    \author Denis Megerle
    """
    return "" #Having no ID is not allowed. However this will be handled in C++.

  cpdef unload(self):
    """
    \brief This method releases the current plugin.
    Essentially it overrides the run method to stop the plugin from running each
    cycle.

    \version 0.5.0
    \author Denis Megerle
    """
    pass  ##TODO: implement

  ########################################################################################

  ##################################Acessor Methods#######################################

  cpdef getCycle(self):
    """
    \brief Method gives the current cycle to work on.

    \returns the python cycle representation

    \version 0.5.0
    \author Denis Megerle
    """
    return Cycle.Cycle()

  cpdef addEvent(self, key, value):
    """
    \brief This method adds an event to the current cycle. The event to be added is coded as a key (event type) and a string (additional event information). Events can be anything and will be added to the cycle.

    \param key Key of Events, using the EventEnum class for further information
    \param value String of additional information, formation depends on specific event. Formatted as pythondict.

    \returns a bool stating whether the event has been added

    \version 0.5.0
    \author Denis Megerle
    """
    cdef char* c_value
    if isinstance(key, int) and isinstance(value, str):
      py_byte_string = value.encode('UTF-8')
      c_value = py_byte_string
      return self.getPythonPlugin().addPyEvent(key, c_value)
    return False;

  cdef registerCycleStorage(self, index, type):
    """
    \brief Registering a cycle storage to be used as storage container for each cycle

    \returns a bool whether the storage has been added or not

    \version 0.5.0
    \author Denis Megerle
    """
    cdef const char* c_type
    cdef char* c_index

    if (type == bool):
      c_type = "bool"
    elif (type == str):
      c_type = "str"
    elif (type == int):
      c_type = "int"

    if isinstance(index, str):
      py_byte_string = index.encode('UTF-8')
      c_index = py_byte_string
      return self.getPythonPlugin().registerPyCycleStorage(c_index, c_type)
    return False;

  cpdef registerInt(self, index):
    return self.registerCycleStorage(index, int)

  cpdef registerString(self, index):
    return self.registerCycleStorage(index, str)

  cpdef registerBool(self, index):
    return self.registerCycleStorage(index, bool)
  ########################################################################################

  #Private elper method getting the corresponding pythonplugin############################
  cdef CPlugin.PythonPlugin* getPythonPlugin(self):
    py_byte_string = self.getID().encode('UTF-8')
    cdef char* c_string = py_byte_string
    return CPlugin.PythonPlugin.getPythonPlugin(c_string)
  #####################################################################################END
