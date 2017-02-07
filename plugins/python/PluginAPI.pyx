cimport
"""
\brief The plugin api provides some basic functionality and convenience method
for user plugins.
It interacts with the backend and modifies / adds events and additional user
data.

:version: 0.0.1
:author: Denis Megerle
"""

cdef Cycle* currentCycle = getCurrentCycle()

cdef getCurrentCycle():
	return Cycle.
def getCycle(number):
	"""
	\brief This method gets a cycle at any given moment.
	The capture starts at 0, so a plugin can technically access any cycle (and
	therefore also compare cycles).
	However this method should be called very rarely since it recalculates the 		cycle
	based on snapshots.
     
    \returns a the cycle at the given moment

	@param unsigned int number : The cycle number to be retrieved. 0 is 		representing the first cycle.
	@return Cycle :
	@author
	"""
	## TODO: implement ##
	return 0
	
def getAmountOfCN():
	"""
	\brief This method returns the amount of controlled nodes currently active.
	The amount of controlled nodes is retrieved from a specific cycle, thus
	representing the CNs active at that given cycle.
     
	\returns number of CNs in the current cycle

	@return unsigned int :
	@author
	"""
	
	## TODO: implement ##
	pass
	
def addEvent(key, value):
	"""
	\brief This method adds an event to the current cycle.
	The event to be added is coded as a key (event type) and a string 		(additional
	event information). Events can be anything and will be added to the cycle.
     
	\returns a bool stating whether the event has been added

	@param unsigned int key : Key of Events, using the EventEnum class for 		further information
	@param string value : String of additional information, formation depends 		on specific event.
						  Formatted as pythondict.
	@return  :
	@author
	"""
	
	## TODO: implement ##
	pass
	
def addData(key, data):
	"""
	\brief This method allows the user to add specific data to cycles.
	Other data can be added to any cycle, that might be processed by other parts of
	the program, like the user interface.
     
	\returns whether the data has been successfully added

	@param unsigned int key : The key this data shall be associated with.
	@param string data : Data to be added as a string
	@return bool :
	@author
	"""
	
	## TODO: implement ##
	pass

def addFilter(filters):
	"""
	\brief This method adds a filter for specific ODs or other data.

	@param std::vector<std::string> filters : A list of strings to be shown in 		the graphical user interface.
	@return bool :
	@author
	"""
	
	## TODO: implement ##
	pass
