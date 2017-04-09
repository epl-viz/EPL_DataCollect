cimport CCycle
cimport CPlugin
cimport Cycle
cimport CEvents
import Cycle
import importlib
import sys
from libcpp.string cimport string

cdef class Plugin:
  """This class represents a custom user plugin base.

  Users can inherit from this class and therefore create their own plugins.
  Plugins use the run method, which run each cycle allowing the user to check for
  data each cycle.
  """

  def __cinit__(self):
    pass

  ###########################Template Methods#############################################
  cpdef initialize(self):
    """Initialize method of a plugin. (OPTIONAL: OVERWRITE)

    The initialize method will only be called once, at the moment the plugin
    gets bound.
    It therefore is used to create datastructures / prepare the plugin for running
    at each cycle and for adding the ID and dependencies.
    By default this will return nothing, since not initializing a plugin is legit.

    .. note::
      This method should be overwritten to create a custom initialize for your plugin.
    """
    return True  #initialize can be empty

  cpdef getID(self):
    """Getting ID, namely the Plugins name.

    .. note::
      This method should be overwritten to create a custom initialize for your plugin.
    """
    return type(self).__name__  #initialize can be empty

  cpdef run(self):
    """Run method of a plugin. (OPTIONAL: OVERWRITE)

    This method runs each cycle, here is the user plugins main.
    Users code will be executed each cycle thus this method should be kept quick. If
    this method runs to slow it will be stopped and rerun to prevent slowing down of
    the program.
    """
    pass  #if no run implemented

  cpdef getDependencies(self):
    """Dependencies (other plugins to specifiy) (OPTIONAL: OVERWRITE)

    This method returns the dependencies of the plugin as a string.
    Dependencies are splitted by whitespace and the plugin will only be started if
    each dependency is resolved.

    :return: a string of dependencies
    :rtype: python str
    """
    return "" #Dependencies can be empty if not needed

  cpdef unload(self):
    """Stopping current plugin.

    This method releases the current plugin.
    Essentially it overrides the run method to stop the plugin from running each
    cycle.
    """
    self.addEvent(<int>CEvents.VIEW_EV_TEXT, "Unloading plugin" + self.getID(), "")
    self.getPythonPlugin().setRunning(False)

  ########################################################################################

  ##################################Acessor Methods#######################################

  def getCycle(self):
    """Getting current cycle.

    Method gives the current cycle to work on.

    :return: the python cycle representation
    :rtype: @Cycle.Cycle
    """
    return Cycle.createCycle(self.getPythonPlugin().getCurrentCycle())

  def getCycleByNum(self, number):
    """Getting a specific cycle.

    This method gets a cycle at any given moment. The capture starts at 0, so a plugin can technically access any cycle
    (and therefore also compare cycles).

    :param number: the number of the cycle
    :type number: python int

    :return: the python cycle representation
    :rtype: @Cycle.Cycle

    .. note::
      However this method should be called very rarely since it recalculates the cycle based on snapshots hence it's very cpu intensive.
    """
    if isinstance(number, int):
      return Cycle.createCycle(self.getPythonPlugin().getCycleByNum(number))

  def addEvent(self, key, value, arg):
    """Adds an event by key, value and arg.

    This method adds an event to the current cycle. The event to be added is coded as a key (event type) and a string (additional event information). Events can be anything and will be added to the cycle.

    :param key: Key of Events, using the EventEnum class for further information
    :param value: String of additional information, formation depends on specific event. Formatted as pythondict.
    :param arg: string of arguments
    :type key: integer
    :type value: python str
    :type arg: python str

    :return: a bool stating whether the event has been added
    :rtype: python bool

    .. note::
      Event keys by Events.<EVENT>.value
    """
    if isinstance(key, int) and isinstance(value, str) and isinstance(arg, str):
      return self.getPythonPlugin().addPyEvent(key, value.encode('utf-8'), arg.encode('utf-8'))
    return False;

  # internal method
  cdef registerCycleStorage(self, index, type):
    cdef int c_type

    if (type == int):
      c_type = 1
    elif (type == str):
      c_type = 2

    if isinstance(index, str):
      return self.getPythonPlugin().registerPyCycleStorage(index.encode('utf-8'), c_type)
    return False;

  cpdef registerInt(self, index):
    """Registering a global cycle storage integer

    Registering a cycle storage to be used as storage container for each cycle

    :param index: index str of storage
    :type index: python str

    :return: a bool whether the storage has been added or not
    :rtype: python bool

    .. note::
      Cycle storages are plugin global, all plugins can access this storage
    """
    return self.registerCycleStorage(index, int)

  cpdef registerStr(self, index):
    """Registering a global cycle storage string

    Registering a cycle storage to be used as storage container for each cycle

    :param index: index str of storage
    :type index: python str

    :return: a bool whether the storage has been added or not
    :rtype: python bool

    .. note::
      Cycle storages are plugin global, all plugins can access this storage
    """
    return self.registerCycleStorage(index, str)

  cpdef getStorage(self, index): #THIS METHOD returns stuff from the own storage of the plugin
    """Getting plugin internal storage

    This method allows the user to get Data from the plugin internal storage as string

    :param index: The key of the data as str
    :type index: python str

    :return: the data as str
    :rtype: python str
    """
    if isinstance(index, str):
      return self.getPythonPlugin().getStorage(index.encode('utf-8')).decode() #returning None type if not found !

  cpdef setStorage(self, index, var):
    """Setting a plugin internal storage

    This method allows the user to save a var at an index, plugin internal.

    :param index: The key this data shall be associated with.
    :param var: Data to be added as a string
    :type index: python str
    :type var: python str

    :return: whether the data has been successfully added
    """
    if isinstance(index, str) and isinstance(var, str):
      return self.getPythonPlugin().setStorage(index.encode('utf-8'), var.encode('utf-8'))
    return False

  cpdef getData(self, index):
    """Getting a data from cycle storage

    This method allows the user to get specific data. Other data can be added to any cycle, that might be processed by other parts of the program, like the user interface. This data is program global !

    :param index: The key this data is associated with
    :type index: python str

    :return: the data as str
    :rtype: python str
    """
    if isinstance(index, str):
      data = self.getPythonPlugin().getData(index.encode('utf-8'))
      return data.decode()
    # return None if not available

  cpdef setData(self, index, var):
    """Setting a data to cycle storage

    This method allows the user to add specific data to cycles. Other data can be added to any cycle, that might be processed by other parts of the program, like the user interface.

    :param index: The key this data shall be associated with.
    :param var: Data to be added as a string
    :type index: python str
    :type var: python str

    :return: whether the data has been successfully added
    :rtype: python str
    """
    if isinstance(index, str):
      if isinstance(var, str):
        return self.getPythonPlugin().setDataStr(index.encode('utf-8'), var.encode('utf-8'))
      elif isinstance(var, int):
        return self.getPythonPlugin().setDataInt(index.encode('utf-8'), var)
    return False

  cpdef requestFilter(self, typeEnu):
    """Requesting a filter

    Requesting a filter of specific type (FiltersEnum)

    :param typeEnu: enum filter type
    :type typeEnu: integer from Filters enum

    .. note::
      A filter has to be requested per plugin, then one can add filter entries via addFilterEntry or the api.
    """
    if isinstance(typeEnu, int):
      return self.getPythonPlugin().requestFilter(typeEnu)

  cpdef addFilterEntry(self, filter):
    """Adding a filter od entry.

    Adding a filter, use with integer 0xabcd.

    .. note::
      A filter has to be requested first.

    .. seealso:: requestFilter
    """
    if isinstance(filter, int):
      self.getPythonPlugin().addFilterEntry(filter);
  ########################################################################################

  #Private helper method getting the corresponding pythonplugin############################
  cdef CPlugin.PythonPlugin* getPythonPlugin(self):
    return CPlugin.PythonPlugin.getPythonPlugin(self.getID().encode('utf-8'))

  #####################################################################################END
