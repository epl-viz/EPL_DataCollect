from libcpp cimport bool
from libcpp.string cimport string
from libcpp.map cimport map
from libcpp.pair cimport pair
from libcpp.vector cimport vector
import time

## PyInit -> initmodule -> run function from c++ for test use ##




# test code to run from c++ ---------
cdef public double cy_fct(double val):
	return val

######### 1. create class to use from c++, 2. write constructor function,
######### 3. write wrapper function to use those from c++...
cdef public class PyPlug[object PyPlug, type PyType]:
	def __cinit__(self):
		pass
	cdef public int run(self):
		return 52

## constructor function for c++
cdef public PyPlug buildPyPlug():
	return PyPlug()

## wrapper functions for c
cdef public int run_wrapper(PyPlug obj):
	return obj.run()
# -------------------------------end

cdef class PyPlugin:
	cdef int a
	cdef bla(self):
		print("bla function running")


# --> TODO: create cdef class that can be inited from C++ !
# --> TODO: in this cdef class create a constructor, the cdef class needs to have a python
			#class as attribute. in the constructor instanciate this pyclass with the
			#given Plugin.
# --> TODO: add glue methods, redirect the run method of cdef class to the attribute pyclass.run 				#class!




# THIS IS THE ACCORDING PYTHON CLASS
class Plugin (object):

	#TEST CONSTR
	def __init__(self):
		self.ident = time.time()
	
	"""
	\brief This class represents a custom user plugin base.
	Users can inherit from this class and therefore create their own plugins.
	Plugins use the run method, which run each cycle allowing the user to check for
	data each cycle.

	:version:
	:author:
	"""

	""" ATTRIBUTES

	A unique(!) ID to represent the plugin in the system.

	id  (private)

	The dependencies of this plugin as a string

	dependencies  (private)

	"""

	def initialize(self):
		"""
		\brief The initialize method will only be called once, at the moment the plugin
		gets bound.
		It therefore is used to create datastructures / prepare the plugin for running
		at each cycle and for adding the ID and dependencies.
	
		@return  :
		@author
		"""
		pass

	def run(self, currentCycle):
		"""
		\brief This method runs each cycle, here is the user plugins main.
		Users code will be executed each cycle thus this method should be kept quick. If
		this method runs to slow it will be stopped and rerun to prevent slowing down of
		the program.
	
		@param Cycle currentCycle : The cycle to be worked with, usually the newest cycle in the 			system.
		@return  :
		@author
		"""
		
		
		print("RUN FUNCTION OF ", self.ident)
	
	def getDependencies(self):
		"""
		\brief This method returns the dependencies of the plugin as a string.
		Dependencies are splitted by whitespace and the plugin will only be started if
		each dependency is resolved.
		     
		\returns a string of dependencies
		
		@return string :
		@author
		"""
		pass
	
	def getID(self):
		"""
		\brief This method returns the unique ID of this plugin.
		   
		\returns the string of the ID
		
		@return string :
		@author
		"""
		pass
		
	def unload(self):
		"""
		\brief This method releases the current plugin.
		Essentially it overrides the run method to stop the plugin from running each
		cycle.
		     
		
		@return  :
		@author
		"""
		pass
	
#Testmethod
def main():
	print("main running")
	plug = PyPlugin()
	plug.bla()
	plug2 = Plugin()
	plug2.run(0)
