import Plugin
import Cycle
# just a quick test method to check whether calls to all parties work, TODO delete
class PyCycle(Cycle.Cycle):
	pass

#Plugin.main()
pc = PyCycle()
print(pc.getCycleNum())
