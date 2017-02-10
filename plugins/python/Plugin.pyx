cimport CPlugin
import importlib
from libcpp.string cimport string

###########----TESTMETHODS----#############
def main():
  pluggerino = Plugin("pyPlugins.PluginA")
  pluggerino.initialize()
  pluggerino.run()
  print(pluggerino.getDependencies())
  print(pluggerino.getID())
###########----TESTMETHODS----#############

cdef api class Plugin[object PyPlug, type PyType]:
  # TODO: declare cdef Cycle.Cycle() (cython cycle)
  cdef object pyObject  # python object -> either module and run/init or create an object from the class in this module
  
  def __cinit__(self, modname):
    importedModule = importlib.import_module(modname)    
    self.pyObject = importedModule

  cpdef api initialize(self):
    try:
      self.pyObject.initialize()
    except:
      print("Warning: " + self.pyObject.__name__ + " initialize method is missing!\n" \
              + "Skipping...")      # no init has to be given since there might be applications that don't need it

  cpdef api int run(self):
    return 52   ##TODO: update cycle* of declared and start run method of pyObject.

  cpdef api getDependencies(self):
    try:
      return self.pyObject.getDependencies()
    except:
      print("Warning: " + self.pyObject.__name__ + " getDependencies method is missing!\n" \
             +"Will be treated as no dependencies...")
      return ""

  cpdef api getID(self):
    try:
      return self.pyObject.getID()
    except:
      print("Error: " + self.pyObject.__name__ + " getID method is missing!\n" \
             +"A plugin needs an ID!")







## Constructor function for C++ TODO: test in c++ for string conversion
cdef api Plugin buildPlugin(name):
  return Plugin(name)

## Wrapper functions for C++
cdef api int run(Plugin obj):
  return obj.run()
# -------------------------------end


# THIS IS THE ACCORDING PYTHON CLASS
class PyPlugin (object):

  #TEST CONSTR
  def __init__(self):
    self.ident = "--PyPlugin--"

  """
  \brief This class represents a custom user plugin base.
  Users can inherit from this class and therefore create their own plugins.
  Plugins use the run method, which run each cycle allowing the user to check for
  data each cycle.

  :version:
  :author:
  """

  """ ATTRIBUTES

  A unique(!) ID to represent the plugin in the system.

  id  (private)

  The dependencies of this plugin as a string

  dependencies  (private)

  """

  def initialize(self):
    """
    \brief The initialize method will only be called once, at the moment the plugin
    gets bound.
    It therefore is used to create datastructures / prepare the plugin for running
    at each cycle and for adding the ID and dependencies.

    @return  :
    @author
    """
    pass

  def run(self, currentCycle):
    """
    \brief This method runs each cycle, here is the user plugins main.
    Users code will be executed each cycle thus this method should be kept quick. If
    this method runs to slow it will be stopped and rerun to prevent slowing down of
    the program.

    @param Cycle currentCycle : The cycle to be worked with, usually the newest cycle in the 			system.
    @return  :
    @author
    """


    print("RUN FUNCTION OF ", self.ident)

  def getDependencies(self):
    """
    \brief This method returns the dependencies of the plugin as a string.
    Dependencies are splitted by whitespace and the plugin will only be started if
    each dependency is resolved.

    \returns a string of dependencies

    @return string :
    @author
    """
    pass

  def getID(self):
    """
    \brief This method returns the unique ID of this plugin.

    \returns the string of the ID

    @return string :
    @author
    """
    pass

  def unload(self):
    """
    \brief This method releases the current plugin.
    Essentially it overrides the run method to stop the plugin from running each
    cycle.


    @return  :
    @author
    """
    pass


# test code to run from c++ ---------TODO: Delete
cdef api double cy_fct(double val):
  return val
