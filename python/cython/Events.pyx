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

cpdef enum Events:
  EV_STARTCAP = <int>EvType.VIEW_STARTCAP
  EV_ENDCAP = <int>EvType.VIEW_ENDCAP
  EV_HIGHLIGHT_MN = <int>EvType.VIEW_EV_HIGHLIGHT_MN
  EV_HIGHLIGHT_CN = <int>EvType.VIEW_EV_HIGHLIGHT_CN
  EV_HIGHLIGHT_OD_ENTRY = <int>EvType.VIEW_EV_HIGHLIGHT_OD_ENTRY
  EV_JUMPTOTIME = <int>EvType.VIEW_EV_JUMPTOTIME
  EV_TEXT = <int>EvType.VIEW_EV_TEXT
  EV_OTHER = <int>EvType.PLUGIN_OTHER
