import Plugin
import math

class Sample_CSUsage(Plugin.Plugin):
  def getID(self):
    return "Sample_CSUsage"

  def initialize(self):
    return self.registerInt("MyIntStorage")

  def run(self):
    current = int(self.getData("MyIntStorage")) # it's an int storage, it will always be castable to int
    next = current + 100 * math.sin(self.getCycle().getCycleNum())
    self.setData("MyIntStorage", int(next))
