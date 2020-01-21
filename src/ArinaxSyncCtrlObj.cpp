#include <sstream>
#include "ArinaxSyncCtrlObj.h"
#include "ArinaxCamera.h"

using namespace lima;
using namespace lima::Arinax;

SyncCtrlObj::SyncCtrlObj(Camera *cam) :
  m_cam(cam),
  m_handle(cam->getHandle()),
  m_trig_mode(IntTrig),
  m_nb_frames(1),
  m_started(false)
{
  DEB_CONSTRUCTOR();
  m_access_mode = cam->m_as_master ? 
    HwSyncCtrlObj::Master : HwSyncCtrlObj::Monitor;
}

SyncCtrlObj::~SyncCtrlObj()
{
  DEB_DESTRUCTOR();
}

bool SyncCtrlObj::checkTrigMode(TrigMode trig_mode)
{
  DEB_MEMBER_FUNCT();
  DEB_PARAM() << DEB_VAR1(trig_mode);

  switch(trig_mode)
    {
    case IntTrig:
    case ExtTrigMult:
      return true;
    default:
      return false;
    }
}

void SyncCtrlObj::setTrigMode(TrigMode trig_mode)
{
  DEB_MEMBER_FUNCT();
  DEB_PARAM() << DEB_VAR1(trig_mode);
/*
  tPvErr error;
  if(checkTrigMode(trig_mode))
    {
      switch(trig_mode)
	{
	case ExtTrigMult:
	  error = PvAttrEnumSet(m_handle, "FrameStartTriggerMode", "SyncIn1");
	  if(error)
	    throw LIMA_HW_EXC(Error,"Can't set trigger input");
	  error = PvAttrEnumSet(m_handle, "FrameStartTriggerEvent", "EdgeRising");
	  if(error)
	    throw LIMA_HW_EXC(Error,"Can't change Trigger start to a rising edge");
	  break;
	default:		// Software
	  error = PvAttrEnumSet(m_handle, "FrameStartTriggerMode", "FixedRate");
	  if(error)
	    {
	      std::stringstream message;
	      message << "could not set trigger mode to FixedRate " << error;
	      throw LIMA_HW_EXC(Error,message.str().c_str());
	    }
	  break;
	}*/
      m_trig_mode = trig_mode;
/*    }
  else
    throw LIMA_HW_EXC(NotSupported,"Trigger type not supported");
*/
}

void SyncCtrlObj::getTrigMode(TrigMode &trig_mode)
{
  trig_mode = m_trig_mode;
}

void SyncCtrlObj::setExpTime(double exp_time)
{
  DEB_MEMBER_FUNCT();
  DEB_PARAM() << DEB_VAR1(exp_time);
  int32_t error = arinax_ns::setExposure(m_handle, exp_time);
  if(error)
    throw LIMA_HW_EXC(Error,"Can't set exposure");
}

void SyncCtrlObj::getExpTime(double &exp_time)
{
  DEB_MEMBER_FUNCT();
  double exp;
  arinax_ns::getExposure(m_handle, &exp);
  exp_time = exp;
  DEB_RETURN() << DEB_VAR1(exp_time);
}

void SyncCtrlObj::setLatTime(double  lat_time)
{
  //No latency managed
}

void SyncCtrlObj::getLatTime(double& lat_time)
{
  lat_time = 0.;		// Don't know
}

void SyncCtrlObj::setNbFrames(int  nb_frames)
{
  DEB_MEMBER_FUNCT();
  DEB_PARAM() << DEB_VAR1(nb_frames);

  m_nb_frames = nb_frames;
}

void SyncCtrlObj::getNbFrames(int& nb_frames)
{
  nb_frames = m_nb_frames;
}

void SyncCtrlObj::setNbHwFrames(int  nb_frames)
{
  setNbFrames(nb_frames);
}

void SyncCtrlObj::getNbHwFrames(int& nb_frames)
{
  getNbFrames(nb_frames);
}

void SyncCtrlObj::getValidRanges(ValidRangesType& valid_ranges)
{
  valid_ranges.min_exp_time = 1e-6; // Don't know
  valid_ranges.max_exp_time = 60.; // Don't know
  valid_ranges.min_lat_time = 0.; // Don't know
  valid_ranges.max_lat_time = 0.; // Don't know
}

