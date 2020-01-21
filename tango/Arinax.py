############################################################################
# This file is part of LImA, a Library for Image Acquisition
#
# Copyright (C) : 2009-2019
# European Synchrotron Radiation Facility
# BP 220, Grenoble 38043
# FRANCE
#
# This is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, see <http://www.gnu.org/licenses/>.
############################################################################
#=============================================================================
#
# file :        Arinax.py
#
# description : Python source for the Arinax video server and its commands.
#                The class is derived from Device. It represents the
#                CORBA servant object which will be accessed from the
#                network. All commands which can be executed on the
#                Arinax video server are implemented in this file.
#
# project :     TANGO Device Server
#
# copyleft :    European Synchrotron Radiation Facility
#               BP 220, Grenoble 38043
#               FRANCE
#
#=============================================================================
#         (c) - Bliss - ESRF
#=============================================================================
#
import PyTango
from Lima import Core
from Lima import Arinax as ArinaxAcq
from Lima.Server import AttrHelper


class Arinax(PyTango.Device_4Impl):

    Core.DEB_CLASS(Core.DebModApplication, 'LimaCCDs')


#------------------------------------------------------------------
#    Device constructor
#------------------------------------------------------------------
    def __init__(self,*args) :
        PyTango.Device_4Impl.__init__(self,*args)

        self.init_device()

#------------------------------------------------------------------
#    Device destructor
#------------------------------------------------------------------
    def delete_device(self):
        pass

#------------------------------------------------------------------
#    Device initialization
#------------------------------------------------------------------
    @Core.DEB_MEMBER_FUNCT
    def init_device(self):
        self.set_state(PyTango.DevState.ON)
        self.get_device_properties(self.get_device_class())

    @Core.DEB_MEMBER_FUNCT
    def getAttrStringValueList(self, attr_name):
        return AttrHelper.get_attr_string_value_list(self, attr_name)

    @Core.DEB_MEMBER_FUNCT
    def scale_for_zoom(self, idx):
        return _ArinaxCam.getScale(idx)

    def __getattr__(self,name) :
        return AttrHelper.get_attr_4u(self, name, ArinaxAcq)

#==================================================================
#
#    Arinax video server read/write attribute methods
#
#==================================================================

#------------------------------------------------------------------
#    Read zoom_percent attribute
#------------------------------------------------------------------
    def read_zoom_percent(self, attr):
        zoom = _ArinaxCam.getZoomLevel()
        if zoom is None:
            zoom = 0.
        attr.set_value(zoom)

#------------------------------------------------------------------
#    Write zoom_percent attribute
#------------------------------------------------------------------
    def write_zoom_percent(self, attr):
        data = attr.get_write_value()
        _ArinaxCam.setZoomLevel(data)

#------------------------------------------------------------------
#    Read zoom_index attribute
#------------------------------------------------------------------
    def read_zoom_index(self, attr):
        zoom = _ArinaxCam.getZoom()
        if zoom is None:
            zoom = 1
        attr.set_value(zoom)

#------------------------------------------------------------------
#    Write zoom_index attribute
#------------------------------------------------------------------
    def write_zoom_index(self, attr):
        data = attr.get_write_value()
        _ArinaxCam.setZoom(data)

#------------------------------------------------------------------
#    Read zoom_index_limits attribute
#------------------------------------------------------------------
    def read_zoom_index_limits(self, attr):
        zoom = _ArinaxCam.getZoomRange()
        if zoom is None:
            zoom = []
        attr.set_value(zoom)

#------------------------------------------------------------------
#    Read predefined_zoom_levels attribute
#------------------------------------------------------------------
    def read_predefined_zoom_levels(self, attr):
        zoom = _ArinaxCam.getPredefinedZoomLevels()
        if zoom is None:
            zoom = []
        attr.set_value(zoom)

#------------------------------------------------------------------
#    Read scale attribute
#------------------------------------------------------------------
    def read_scale(self, attr):
        scale = _ArinaxCam.getScale()
        attr.set_value(scale)

#------------------------------------------------------------------
#    Read scale_px_mm attribute
#------------------------------------------------------------------
    def read_scale_px_mm(self, attr):
        scale = _ArinaxCam.getScale()
        attr.set_value(1000./scale)

class ArinaxClass(PyTango.DeviceClass):

    class_property_list = {}

    device_property_list = {
        'cam_name':
        [PyTango.DevString,
         "Camera name",[]],
        }

    cmd_list = {
        'getAttrStringValueList':
        [[PyTango.DevString, "Attribute name"],
         [PyTango.DevVarStringArray, "Authorized String value list"]],
        'scale_for_zoom':
        [[PyTango.DevLong, "Index"],
         [PyTango.DevDouble, "Scale for specified zoom level"]],
        }

    attr_list = {
      'zoom_percent':
        [[PyTango.DevDouble,
           PyTango.SCALAR,
           PyTango.READ_WRITE]],
      'zoom_index':
        [[PyTango.DevLong,
           PyTango.SCALAR,
           PyTango.READ_WRITE]],
      'zoom_index_limits':
        [[PyTango.DevLong,
           PyTango.SPECTRUM,
           PyTango.READ, 2]],
      'predefined_zoom_levels':
        [[PyTango.DevDouble,
           PyTango.SPECTRUM,
           PyTango.READ, 15]],
      'scale':
        [[PyTango.DevDouble,
           PyTango.SCALAR,
           PyTango.READ]],
      'scale_px_mm':
        [[PyTango.DevDouble,
           PyTango.SCALAR,
           PyTango.READ]],
      }

    def __init__(self,name) :
        PyTango.DeviceClass.__init__(self,name)
        self.set_type(name)

#----------------------------------------------------------------------------
# Plugins
#----------------------------------------------------------------------------
_ArinaxCam = None
_ArinaxInterface = None

def get_control(cam_name = "",**keys) :
    print ("cam_name",cam_name)
    global _ArinaxCam
    global _ArinaxInterface
    if _ArinaxCam is None:
        _ArinaxCam = ArinaxAcq.Camera(cam_name)
        _ArinaxInterface = ArinaxAcq.Interface(_ArinaxCam)
    return Core.CtControl(_ArinaxInterface)

def get_tango_specific_class_n_device():
    return ArinaxClass,Arinax
