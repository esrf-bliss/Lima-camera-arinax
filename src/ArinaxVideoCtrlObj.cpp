#include "ArinaxVideoCtrlObj.h"
#include "ArinaxCamera.h"
#include "ArinaxSyncCtrlObj.h"

using namespace lima;
using namespace lima::Arinax;

const uint32_t MAX_GAIN = 29;

VideoCtrlObj::VideoCtrlObj(Camera *cam) :
  m_cam(cam),
  m_handle(cam->getHandle()),
  m_live(false)
{
}

VideoCtrlObj::~VideoCtrlObj()
{
}

void VideoCtrlObj::getSupportedVideoMode(std::list<VideoMode> &aList) const
{
  aList.push_back(Y8);
  if(!m_cam->isMonochrome())
    {
      aList.push_back(BAYER_RG8);
      aList.push_back(BAYER_RG16);
      aList.push_back(RGB24);
      aList.push_back(BGR24);
    }
  else
    aList.push_back(Y16);
}

void VideoCtrlObj::getVideoMode(VideoMode &aMode) const
{
  aMode = m_cam->getVideoMode();
}

void VideoCtrlObj::setVideoMode(VideoMode aMode)
{
  m_cam->setVideoMode(aMode);
}

void VideoCtrlObj::setLive(bool flag)
{
  m_live = flag;
  if(flag)
    {
      m_cam->_allocBuffer();
      m_cam->startAcq();
    }
  else
    m_cam->stopAcq();
}

void VideoCtrlObj::getLive(bool &flag) const
{
  flag = m_live;
}

void VideoCtrlObj::getGain(double &aGain) const
{
  double localGain;
  int32_t error=arinax_ns::getGain(m_handle, &localGain);
  aGain = localGain;
}

void VideoCtrlObj::setGain(double aGain)
{
  int32_t error=arinax_ns::setGain(m_handle, aGain);
  if(error)
    throw LIMA_HW_EXC(Error,"Can't set gain to asked value");
}

void VideoCtrlObj::checkBin(Bin& bin)
{
  bin = Bin(1,1);		// Do not manage Hw Bin
}

void VideoCtrlObj::checkRoi(const Roi&, Roi& hw_roi)
{
  ImageRaw info;
  
  arinax_ns::getImageInfo(m_handle, &info);

  hw_roi = Roi(0,0, info.m_width, info.m_height); // Do not manage Hw Roi
}
