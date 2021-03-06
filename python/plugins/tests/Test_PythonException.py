import Plugin                             # make sure Plugin cython lib is imported
import unittest

class Test_PythonException(Plugin.Plugin, unittest.TestCase):       # name your class plugin like your python module, and let it derive from Plugin.Plugin !
  def getID(self):
    return "Test_PythonException"                # make sure the getID method returns the exact class name as a str

  def initialize(self):                   # (optional) a initialize method, returning true
    return True                             # register your cyclestorages and needed data here

  def run(self):                          # (optional, but without it well...) run method
    print("AN EXCEPTION WITH TRACEBACK SHOULD BE THROWN")
    i = int(23, 23, 23, 23, 23)
    if self.getCycle().getCycleNum() > 1:
      self.fail("[python] ERROR exceptions should stop the specific plugin !")

  def getDependencies(self):              # (optional) this method takes other plugin names as str and makes sure those are there, too
    return ""

