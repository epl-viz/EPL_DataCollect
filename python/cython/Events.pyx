cimport CEvents

#Representation of Events to be used in the plugin
cpdef enum Events:
  EV_STARTCAP = <int>CEvents.EvType.VIEW_STARTCAP
  EV_ENDCAP = <int>CEvents.EvType.VIEW_ENDCAP
  EV_HIGHLIGHT_MN = <int>CEvents.EvType.VIEW_EV_HIGHLIGHT_MN
  EV_HIGHLIGHT_CN = <int>CEvents.EvType.VIEW_EV_HIGHLIGHT_CN
  EV_HIGHLIGHT_OD_ENTRY = <int>CEvents.EvType.VIEW_EV_HIGHLIGHT_OD_ENTRY
  EV_JUMPTOTIME = <int>CEvents.EvType.VIEW_EV_JUMPTOTIME
  EV_TEXT = <int>CEvents.EvType.VIEW_EV_TEXT
  EV_OTHER = <int>CEvents.EvType.PLUGIN_OTHER

  # misc events
  EV_PROTO_ERROR = <int>CEvents.EvType.PROTO_ERROR
  EV_ERROR = <int>CEvents.EvType.ERROR
  EV_WARNING = <int>CEvents.EvType.WARNING
  EV_INFO = <int>CEvents.EvType.INFO
  EV_DEBUG = <int>CEvents.EvType.DEBUG
