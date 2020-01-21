#ifndef ARINAXINTERFACE_H
#define ARINAXINTERFACE_H

#include "lima/HwInterface.h"

namespace lima
{
  namespace Arinax
  {
    class Camera;
    class DetInfoCtrlObj;
    class VideoCtrlObj;
    class SyncCtrlObj;
    class BinCtrlObj;

    class Interface : public HwInterface
    {
      DEB_CLASS_NAMESPC(DebModCamera, "Interface", "Arinax");

    public:
      Interface(Camera*);
      virtual ~Interface();

      virtual void getCapList(CapList &) const;

      virtual void reset(ResetLevel reset_level);
      virtual void prepareAcq();
      virtual void startAcq();
      virtual void stopAcq();
      virtual void getStatus(StatusType& status);

      virtual int getNbAcquiredFrames();
      virtual int getNbHwAcquiredFrames();
    private:
      Camera* 		m_cam;
      DetInfoCtrlObj* 	m_det_info;
      VideoCtrlObj* 	m_video;
      SyncCtrlObj* 	m_sync;
      BinCtrlObj*       m_bin;
    };

  } // namespace Arinax

} // namespace lima

#endif // ARINAXINTERFACE_H
