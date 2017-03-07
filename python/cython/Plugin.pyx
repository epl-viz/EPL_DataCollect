cimport CCycle
cimport CPlugin
cimport Cycle
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

  \version 1.0.0
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

    \version 1.0.0
    """
    return True  #initialize can be empty

  cpdef run(self):
    """
    \brief This method runs each cycle, here is the user plugins main.
    Users code will be executed each cycle thus this method should be kept quick. If
    this method runs to slow it will be stopped and rerun to prevent slowing down of
    the program.

    \version 1.0.0
    """
    pass  #if no run implemented

  cpdef getDependencies(self):
    """
    \brief This method returns the dependencies of the plugin as a string.
    Dependencies are splitted by whitespace and the plugin will only be started if
    each dependency is resolved.

    \returns a string of dependencies

    \version 0.5.0
    """
    return "" #Dependencies can be empty if not needed

  cpdef unload(self):
    """
    \brief This method releases the current plugin.
    Essentially it overrides the run method to stop the plugin from running each
    cycle.

    \version 1.0.0
    """
    self.getPythonPlugin().setRunning(False)

  ########################################################################################

  ##################################Acessor Methods#######################################

  def getCycle(self):
    """
    \brief Method gives the current cycle to work on.

    \returns the python cycle representation

    \version 1.0.0
    """
    return Cycle.createCycle(self.getPythonPlugin().getCurrentCycle())

  def getCycleByNum(self, number):
    """
    \brief This method gets a cycle at any given moment. The capture starts at 0, so a plugin can technically access any cycle
    (and therefore also compare cycles). However this method should be called very rarely since it recalculates the cycle based on snapshots.

    \param number the number of the cycle

    \returns the python cycle representation

    \version 1.0.0
    """
    if isinstance(number, int):
      return Cycle.createCycle(self.getPythonPlugin().getCycleByNum(number))

  def addEvent(self, key, value, arg):
    """
    \brief This method adds an event to the current cycle. The event to be added is coded as a key (event type) and a string (additional event information). Events can be anything and will be added to the cycle.

    \param key Key of Events, using the EventEnum class for further information
    \param value String of additional information, formation depends on specific event. Formatted as pythondict.
    \param arg string of arguments

    \returns a bool stating whether the event has been added

    \version 1.0.0
    """
    if isinstance(key, int) and isinstance(value, str) and isinstance(arg, str):
      return self.getPythonPlugin().addPyEvent(key, value.encode('utf-8'), arg.encode('utf-8'))
    return False;

  cdef registerCycleStorage(self, index, type):
    """
    \brief Registering a cycle storage to be used as storage container for each cycle

    \returns a bool whether the storage has been added or not

    \version 1.0.0
    """
    cdef int c_type

    if (type == int):
      c_type = 1
    elif (type == str):
      c_type = 2

    if isinstance(index, str):
      return self.getPythonPlugin().registerPyCycleStorage(index.encode('utf-8'), c_type)
    return False;

  # internal method
  cpdef registerInt(self, index):
    return self.registerCycleStorage(index, int)

  # internal method
  cpdef registerStr(self, index):
    return self.registerCycleStorage(index, str)

  cpdef getStorage(self, index): #THIS METHOD returns stuff from the own storage of the plugin
    """
    \brief This method allows the user to get Data from the plugin internal storage as string

    \param key The key of the data as str

    \returns the data as str
    """
    if isinstance(index, str):
      return self.getPythonPlugin().getStorage(index.encode('utf-8')).decode() #returning None type if not found !

  cpdef setStorage(self, index, var):
    """
    \brief This method allows the user to save a var at an index, plugin internal.

    \param key The key this data shall be associated with.
    \param data Data to be added as a string

    \returns whether the data has been successfully added
    """
    if isinstance(index, str) and isinstance(var, str):
      return self.getPythonPlugin().setStorage(index.encode('utf-8'), var.encode('utf-8'))
    return False

  cpdef getData(self, index):
    """
    \brief This method allows the user to get specific data. Other data can be added to any cycle, that might be processed by other parts of the program, like the user interface. This data is program global !

    \param key The key this data is associated with

    \returns the data as str
    """
    if isinstance(index, str):
      data = self.getPythonPlugin().getData(index.encode('utf-8'))
      return data.decode()
    # return None if not available

  cpdef setData(self, index, var):
    """
    \brief This method allows the user to add specific data to cycles. Other data can be added to any cycle, that might be processed by other parts of the program, like the user interface.

    \param key The key this data shall be associated with.
    \param data Data to be added as a string

    \returns whether the data has been successfully added
    """
    if isinstance(index, str):
      if isinstance(var, str):
        return self.getPythonPlugin().setDataStr(index.encode('utf-8'), var.encode('utf-8'))
      elif isinstance(var, int):
        return self.getPythonPlugin().setDataInt(index.encode('utf-8'), var)
    return False

  cpdef addFilter(self, typeEnu, filters):
    """
    \brief This method allows the user to add specific data to cycles. Other data can be added to any cycle, that might be processed by other parts of the program, like the user interface. Currently only INCLUDING filter are supported

    \param typeEnum enum type integer of filter
    \param filters filters to be added as a string

    \returns whether the data has been successfully added
    """
    if isinstance(filters, str):
      return self.getPythonPlugin().addViewFilter(0, filters.encode('utf-8'))
    return False
  ########################################################################################

  #Private helper method getting the corresponding pythonplugin############################
  cdef CPlugin.PythonPlugin* getPythonPlugin(self):
    return CPlugin.PythonPlugin.getPythonPlugin(self.getID().encode('utf-8'))

  #####################################################################################END
