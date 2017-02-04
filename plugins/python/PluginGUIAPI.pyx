"""
\brief This class enables a plugin user to easily add GUI representation
specific events to highlight or start and stop the representations quickly.
It therefore provides convenience methods that add events to the backend.

:version: 0.0.1
:author: Denis Megerle
"""

# HAVE TO GET PLUGINBASE AND CYCLE in cython -> PluginBase needs to store the current cycle
def startLive():
	"""
	\brief An easy method to use to add the start recording event to the current
	cycle.
	It will just start the live representation of the recording, the recording will
	still go on in the background.
	
	@return  :
	@author
	"""

	## TODO: implement ##
	pass

def stopLive():
	"""
	\brief An easy method to use to add the stop recording event to the current
	cycle.
	It will just stop the live representation of the recording, the recording will
	still go on in the background.
	
	@return  :
	@author
	"""

	## TODO: implement ##
	pass

def highlightNode(node, level):
	"""
	\brief This method highlights a specific node in the graphical representation.
	     
	\returns a bool showing whether the node highlight is successful
	
	@param Node node : The node to be highlighted in the graphical representation.
	@param unsigned int level : The level of highlighting to be set. The level of highlight is a 		positive integer number, the graphical / other representation can use this highlighting number and 		define its own way of highlighting based on the number.
	@return bool :
	@author
	"""

	## TODO: implement ##
	pass

def highlightODEntry(entry, level):
	"""
	\brief Highlights a specific OD Entry in the graphical representations.
	The level of highlight is set by an unsigned integer, the graphical / other
	representation can build its own highlighting based on this number.
	     
	\returns true if highlighting was added successful.
	
	@param ODEntry entry : The entry to be highlighted.
	@param unsigned int level : The level of highlighting to be used in the graphical representations.
	@return bool :
	@author
	"""

	
	pass

def setHeat(node, entry, heat = 0.5):
	"""
	\brief Method adding the heat value to an OD entry or data value.
	  
	The heat value is a value between 0 and 1.
	
	@param Node node : The node to be used
	@param ODEntry entry : The entry / data the value should be assigned in this cycle
	@param double heat : The heatvalue as a double between 0 (cold) and 1 (hot)
	@return  :
	@author
	"""

	## TODO: implement ##
	pass
