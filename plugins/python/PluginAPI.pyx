cimport CCycle
"""
\brief The plugin api provides some basic functionality and convenience method
for user plugins.
It interacts with the backend and modifies / adds events and additional user
data.

:version: 0.0.1
:author: Denis Megerle
"""

# getting the cycle to work on...
cdef CCycle.Cycle* currentCycle = PythonPlugin.getCurrentCycle()

def getCycle(number):
	"""
	\brief This method gets a cycle at any given moment. The capture starts at 0, so a plugin can technically access any cycle (and therefore also compare cycles). However this method should be called very rarely since it recalculates the cycle based on snapshots.
     
    	\returns the 'number'th captured cycle
	"""
	## TODO: implement ##
	pass
	
def getAmountOfCN():
	"""
	\brief This method returns the amount of controlled nodes currently active. The amount of controlled nodes is retrieved from a specific cycle, thus representing the CNs active at that given cycle.
     
	\returns number of CNs in the current cycle
	"""
	return currentCycle.getNumNodes()
	
def addEvent(key, value):
	"""
	\brief This method adds an event to the current cycle. The event to be added is coded as a key (event type) and a string (additional event information). Events can be anything and will be added to the cycle.

	\param key Key of Events, using the EventEnum class for further information
	\param value String of additional information, formation depends on specific event. Formatted as pythondict.

	\returns a bool stating whether the event has been added
	"""
	
	## TODO: implement ##
	pass
	
def addData(key, data):
	"""
	\brief This method allows the user to add specific data to cycles. Other data can be added to any cycle, that might be processed by other parts of the program, like the user interface.
     
	\param key The key this data shall be associated with.
	\param data Data to be added as a string

	\returns whether the data has been successfully added
	"""
	
	## TODO: implement, cycle storage will be registered in cpp, add data will be handled by getting the cyclestroagebase and adding it ##
	pass

def addFilter(filters):
	"""
	\brief This method adds a filter for specific ODs or other data.
	
	\param filters : A list of strings to be shown in the graphical user interface.
	
	\return whether the filter addition has been successful
	"""
	
	## TODO: implement ##
	pass

###import###
cdef extern from "PythonPlugin.cpp" namespace "EPL_DataCollect::plugins":
	cdef cppclass PythonPlugin:
		@staticmethod
		CCycle.Cycle* getCurrentCycle()
