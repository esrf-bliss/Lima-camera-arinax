#ifndef ARINAXVIDEOCTRLOBJ_H
#define ARINAXVIDEOCTRLOBJ_H
#include "Arinax.h"
#include "lima/HwVideoCtrlObj.h"

namespace lima
{
  namespace Arinax
  {
    class Camera;
    class SyncCtrlObj;
    class VideoCtrlObj : public HwVideoCtrlObj
    {
      friend class Interface;
      DEB_CLASS_NAMESPC(DebModCamera,"VideoCtrlObj","Arinax");
    public:
      VideoCtrlObj(Camera* cam);
      virtual ~VideoCtrlObj();
 
      virtual void getSupportedVideoMode(std::list<VideoMode> &aList) const;
      virtual void setVideoMode(VideoMode);
      virtual void getVideoMode(VideoMode&) const;

      virtual void setLive(bool);
      virtual void getLive(bool&) const;

      virtual void getGain(double&) const;
      virtual void setGain(double);

      virtual void checkBin(Bin& bin);
      virtual void checkRoi(const Roi& set_roi, Roi& hw_roi);

      virtual void setBin(const Bin&){};
      virtual void setRoi(const Roi&){};

    private:
      Camera*	 	m_cam;
      int32_t    	m_handle;
      bool	 	m_live;
      SyncCtrlObj* 	m_sync;
    };
  }
}
#endif
