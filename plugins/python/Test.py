import Plugin
import Cycle
import PluginAPI
# just a quick test method to check whether calls to all parties work, TODO delete
class PyCycle(Cycle.Cycle):
  pass

#Plugin.main()
pc = PyCycle()
print(pc.getCycleNum())
print(pc.getActiveEvents())
print(pc.getODEntry(0,0x1000))

PluginAPI.getAmountOfCN()
print("------------PLUGIN TEST-------------")
Plugin.main()
