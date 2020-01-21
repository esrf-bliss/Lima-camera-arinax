#include "ArinaxInterface.h"
#include "ArinaxCamera.h"
#include "ArinaxDetInfoCtrlObj.h"
#include "ArinaxVideoCtrlObj.h"
#include "ArinaxSyncCtrlObj.h"

using namespace lima;
using namespace lima::Arinax;


Interface::Interface(Camera *cam) :
  m_cam(cam)
{
  DEB_CONSTRUCTOR();
  m_det_info = new DetInfoCtrlObj(cam);
  m_video = new VideoCtrlObj(cam);
  cam->_allocBuffer();
  cam->m_video = m_video;
  m_sync = new SyncCtrlObj(cam);
  cam->m_sync = m_sync;

  if(m_video)
    m_video->m_sync = m_sync;
}

Interface::~Interface()
{
  DEB_DESTRUCTOR();
  if(m_video)
    {
      delete m_video;
    }
  delete m_det_info;
  delete m_sync;
}

void Interface::getCapList(CapList &cap_list) const
{
  cap_list.push_back(HwCap(m_sync));
  cap_list.push_back(HwCap(m_det_info));
      cap_list.push_back(HwCap(m_video));
      cap_list.push_back(HwCap(&(m_video->getHwBufferCtrlObj())));
}

void Interface::reset(ResetLevel reset_level)
{
  DEB_MEMBER_FUNCT();
  DEB_PARAM() << DEB_VAR1(reset_level);

  m_cam->stopAcq();
  m_cam->reset();
}

void Interface::prepareAcq()
{
  DEB_MEMBER_FUNCT();
}

void Interface::startAcq()
{
  DEB_MEMBER_FUNCT();

  m_video->getBuffer().setStartTimestamp(Timestamp::now());
  m_cam->startAcq();
}

void Interface::stopAcq()
{
  DEB_MEMBER_FUNCT();

  m_cam->stopAcq();
}

void Interface::getStatus(StatusType& status)
{
  m_cam->getStatus(status);
}

int Interface::getNbAcquiredFrames()
{
  DEB_MEMBER_FUNCT();

  int aNbAcquiredFrames;
  aNbAcquiredFrames = m_cam->getNbAcquiredFrames();

  DEB_RETURN() << DEB_VAR1(aNbAcquiredFrames);
  return aNbAcquiredFrames;
}

int Interface::getNbHwAcquiredFrames()
{
  return getNbAcquiredFrames();
}

