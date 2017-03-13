import Plugin
import Events

###########
# Adding an event each time a certain entry is over threshold x
###########

class Sample_ThresholdEvent(Plugin.Plugin):
  def getID(self):
    return "Sample_ThresholdEvent"

  def run(self):
    cy = self.getCycle()
    try:
      i = int(cy.getODEntry_Sub(1, 0x6200, 1))
      if i >= 20:
        self.addEvent(Events.EV_TEXT.value, "Threshold is reached!!!", "") # provide text as str in first argument
    except (TypeError, ValueError):
      pass  ## not an int here, meaning the odentry is either not inited or smth else
