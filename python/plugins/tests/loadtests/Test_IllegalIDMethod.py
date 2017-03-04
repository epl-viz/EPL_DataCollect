import Plugin

class Test_IllegalIDMethod(Plugin.Plugin):
  def getID(self):
    return 2  # should return string testcase
