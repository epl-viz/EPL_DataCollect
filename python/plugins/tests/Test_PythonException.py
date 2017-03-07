import Plugin                             # make sure Plugin cython lib is imported

class Test_PythonException(Plugin.Plugin):       # name your class plugin like your python module, and let it derive from Plugin.Plugin !
  def getID(self):
    return "Test_PythonException"                # make sure the getID method returns the exact class name as a str

  def initialize(self):                   # (optional) a initialize method, returning true
    return True                             # register your cyclestorages and needed data here

  def run(self):                          # (optional, but without it well...) run method
    i = int(23, 23, 23, 23, 23)

  def getDependencies(self):              # (optional) this method takes other plugin names as str and makes sure those are there, too
    return ""

