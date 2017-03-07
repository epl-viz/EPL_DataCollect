# Event types in c++
cdef extern from "EPLEnums.h" namespace "EPL_DataCollect::EvType":
  cdef enum EvType "EPL_DataCollect::EvType":
    PLUGIN_OTHER,

    # view specific events
    VIEW_STARTCAP,
    VIEW_ENDCAP,
    VIEW_EV_HIGHLIGHT_MN,
    VIEW_EV_HIGHLIGHT_CN,
    VIEW_EV_HIGHLIGHT_OD_ENTRY,
    VIEW_EV_JUMPTOTIME,
    VIEW_EV_TEXT,
    VIEW_EV_IMAGE,
