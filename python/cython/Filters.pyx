#Representation of Filters to be used in the plugin

cdef extern from "EPLEnums.h" namespace "EPL_DataCollect::FilterType":
  cdef enum FilterType "EPL_DataCollect::FilterType":
    INCLUDE,
    EXCLUDE,

cpdef enum Filters:
  INCLUDING = 0
  EXCLUDING = 1
