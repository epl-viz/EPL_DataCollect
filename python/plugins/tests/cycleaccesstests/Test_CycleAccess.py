import Plugin

class Test_CycleAccess(Plugin.Plugin):
  def getID(self):
    return "Test_CycleAccess"

  def run(self):
    cy = self.getCycle()
