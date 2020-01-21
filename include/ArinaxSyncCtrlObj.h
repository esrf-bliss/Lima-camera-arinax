#ifndef ARINAXSYNCCTRLOBJ_H
#define ARINAXSYNCCTRLOBJ_H

#include "Arinax.h"

#include "lima/HwSyncCtrlObj.h"
#include "lima/HwInterface.h"

namespace lima
{
  namespace Arinax
  {
    class Camera;
    class BufferCtrlObj;

    class SyncCtrlObj : public HwSyncCtrlObj
    {
      DEB_CLASS_NAMESPC(DebModCamera,"SyncCtrlObj","Arinax");
    public:
      SyncCtrlObj(Camera*);
      virtual ~SyncCtrlObj();

      virtual bool checkTrigMode(TrigMode trig_mode);
      virtual void setTrigMode(TrigMode  trig_mode);
      virtual void getTrigMode(TrigMode& trig_mode);

      virtual void setExpTime(double  exp_time);
      virtual void getExpTime(double& exp_time);

      virtual void setLatTime(double  lat_time);
      virtual void getLatTime(double& lat_time);

      virtual void setNbFrames(int  nb_frames);
      virtual void getNbFrames(int& nb_frames);

      virtual void setNbHwFrames(int  nb_frames);
      virtual void getNbHwFrames(int& nb_frames);

      virtual void getValidRanges(ValidRangesType& valid_ranges);

    private:
      Camera*		m_cam;
      int32_t	m_handle;
      TrigMode		m_trig_mode;
      int		m_nb_frames;
      bool		m_started;
    };

  } // namespace Arinax
} // namespace lima

#endif // ARINAXSYNCCTRLOBJ_H
