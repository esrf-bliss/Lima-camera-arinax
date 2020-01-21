#ifndef ARINAXBUFFERCTRLOBJ_H
#define ARINAXBUFFERCTRLOBJ_H

#include "Arinax.h"

#include "lima/HwBufferMgr.h"

namespace lima
{
  namespace Arinax
  {
    class Camera;
    class SyncCtrlObj;
    class Interface;

    class BufferCtrlObj : public SoftBufferCtrlObj
    {
      friend class Interface;
      DEB_CLASS_NAMESPC(DebModCamera,"BufferCtrlObj","Arinax");
    public:
      BufferCtrlObj(Camera *cam);
      void prepareAcq();
      void startAcq();
      void getStatus(int32_t &err,bool& exposing) {err = m_status,exposing = m_exposing;}
    private:
      static void _newFrame(ImageRaw*);
      
      int32_t      	m_handle;
      ImageRaw        	m_frame[2];
      SyncCtrlObj* 	m_sync;
      int32_t		m_status;
      bool		m_exposing;
    };
  }
}
#endif
