import Plugin                             # make sure Plugin cython lib is imported

class MinimalPlugin(Plugin.Plugin):       # name your class plugin like your python module, and let it derive from Plugin.Plugin !
  def getID(self):
    return "MinimalPlugin"                # make sure the getID method returns the exact class name as a str

  def initialize(self):                   # (optional) a initialize method, returning true
    return True                             # register your cyclestorages and needed data here

  def run(self):                          # (optional, but without it well...) run method
    pass    ## your code here

  def getDependencies(self):              # (optional) this method takes other plugin names as str and makes sure those are there, too
    return ""
