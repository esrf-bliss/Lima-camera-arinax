Arinax Tango device
=======================

This is the reference documentation of the Arinax Tango device.

you can also find some useful information about the camera models/prerequisite/installation/configuration/compilation in the :ref:`Arinax camera plugin <camera-arinax>` section.

Properties
----------

=============== =============== =============== ==============================================================
Property name	Mandatory	Default value	Description
=============== =============== =============== ==============================================================
cam_name	Yes		N/A		The camera name 
=============== =============== =============== ==============================================================

Attributes
----------
======================= ======= ======================= ===========================================================
Attribute name          RW      Type                    Description
======================= ======= ======================= ===========================================================
predefined_zoom_levels  ro      DevDouble[]             the list of zoom levels 
scale                   ro      DevDouble               the current zoom scale 
scale_px_mm             ro      DevDouble               the pixel to mm scale factor 
zoom_index              rw      DevLong                 the  zoom index 
zoom_index_limits       ro      DevLong[]               the zoom index range 
zoom_percent            rw      DevDouble               the zoom percent 
======================= ======= ======================= ===========================================================
 

Commands
--------

=======================	=============== =======================	===========================================
Command name		Arg. in		Arg. out		Description
=======================	=============== =======================	===========================================
Init			DevVoid 	DevVoid			Do not use
State			DevVoid		DevLong			Return the device state
Status			DevVoid		DevString		Return the device state as a string
getAttrStringValueList	DevString:	DevVarStringArray:	Return the authorized string value list for
			Attribute name	String value list	a given attribute name
scale_for_zoom		DevString	DevVarStringArray	return the scale for passed zoom level 
=======================	=============== =======================	===========================================


