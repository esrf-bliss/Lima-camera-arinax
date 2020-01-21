#include <signal.h>
#include <cstdlib>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "lima/Exceptions.h"

#include "ArinaxCamera.h"
#include "ArinaxSyncCtrlObj.h"
#include "ArinaxVideoCtrlObj.h"

using namespace lima;
using namespace lima::Arinax;

Camera* Camera::a_camera;

Camera::Camera(const std::string& name,bool master,
                bool mono_forced) :
  m_cam_connected(false),
  m_sync(NULL),
  m_video(NULL),
  m_bin(1,1),
  m_mono_forced(mono_forced),
  m_continue_acq(false)
{
  DEB_CONSTRUCTOR();
  //Tango signal management is a real shit (workaround)
  sigset_t signals;
  sigfillset(&signals);
  sigprocmask(SIG_UNBLOCK,&signals,NULL);
  m_predefZoomLock = std::make_shared<std::mutex>();
  m_zoomRangeLock = std::make_shared<std::mutex>();

  // Init Frames
//  m_frame[0].ImageBuffer = NULL;
//  m_frame[0].Context[0] = this;
//  m_frame[1].ImageBuffer = NULL;
//  m_frame[1].Context[0] = this;
  
//  m_camera_name[0] = m_sensor_type[0] = '\0';
//  unsigned long ip = inet_addr(ip_addr.c_str());
  int32_t error = arinax_ns::init(&_camerasNameCBK, &_infoCBK);
  if(error)
    throw LIMA_HW_EXC(Error, "Could not initialize Arinax API");

  m_cam_connected = !arinax_ns::openCameraNotGrab(name.c_str(),
					&m_handle);
  if(!m_cam_connected)
    throw LIMA_HW_EXC(Error, "Camera not found!");

  arinax_ns::registerParameterRangeCallBacks(m_handle, &_doubleRangeParamCBK
          , &_intRangeParamCBK
          , &_stringRangeParamCBK
          , &_zoomLevelRangeCBK
          , &_zoomRangeCBK
          , &_predefZoomCBK
          , &_expoRangeCBK
          , &_gainRangeCBK
          , &_wbRangeCBK);
  arinax_ns::registerParameterCallBacks(m_handle, &_doubleParamCBK
          , &_intParamCBK
          , &_stringParamCBK);
  unsigned long psize;
//  DEB_TRACE() << DEB_VAR3(m_camera_name,m_sensor_type,m_uid);
  ImageRaw info;
  arinax_ns::getImageInfo(m_handle, &info);
  m_maxwidth = info.m_width;
  m_maxheight = info.m_height;

  DEB_TRACE() << DEB_VAR2(m_maxwidth,m_maxheight);


  Bin tmp_bin(1, 1);
  if(master)
    {
      setBin(tmp_bin); // Bin has to be (1,1) for allowing maximum values as width and height

    }
  else
    m_video_mode = Y8;
  m_video_mode = RGB24;  
  m_as_master = master;
  a_camera = this;
}

Camera::~Camera()
{
  DEB_DESTRUCTOR();

  if(m_cam_connected)
    {
      arinax_ns::closeCamera(m_handle);
    }
  arinax_ns::terminateVideoServer();
}

void Camera::_camerasName(int32_t idx, const char* name)
{}

void Camera::_info(int32_t idx, const char* info)
{}
void Camera::_doubleParam(const double, const char *)
{}
void Camera::_intParam(const char *, const int64_t)
{}
void Camera::_stringParam(const char *, const char*)
{}
void Camera::_doubleRangeParam(const char *, int32_t, double*)
{}
void Camera::_intRangeParam(const char *, int32_t, int64_t*)
{}
void Camera::_stringRangeParam(const char *, int32_t, const char**)
{}
void Camera::_zoomLevelRange(int32_t, double*)
{}
void Camera::_zoomRange(int32_t size, int32_t* vals)
{
  std::vector<int> res;
  for(int32_t i = 0; i!= size ; ++i)
  {
      res.push_back(*(static_cast<int32_t*>(vals+i))); 
  }
  m_zoomRangeProm->set_value(res);
}
void Camera::_predefZoom(int32_t size, double* vals)
{
  std::vector<double> res;  
  for(int32_t i = 0; i!= size ; ++i)
  {
      res.push_back(*(static_cast<double*>(vals+i))); 
  }
  m_predefZoomProm->set_value(res);
}
void Camera::_gainRange(int32_t, double*)
{}
void Camera::_expoRange(int32_t, double*)
{}
void Camera::_wbRange(const char *,int32_t, double*)
{}

/** @brief test if the camera is monochrome
 */
bool Camera::isMonochrome() const
{
  DEB_MEMBER_FUNCT();

  return (!strcmp(m_sensor_type,"Mono") || m_mono_forced);
}

VideoMode Camera::getVideoMode() const
{
  DEB_MEMBER_FUNCT();
  DEB_RETURN() << DEB_VAR1(m_video_mode);

  return m_video_mode;
}

void Camera::getCameraName(std::string& name)
{
  DEB_MEMBER_FUNCT();
  DEB_RETURN() << DEB_VAR1(m_camera_name);

  name = m_camera_name;
}
void Camera::setVideoMode(VideoMode aMode)
{
  DEB_MEMBER_FUNCT();
  DEB_PARAM() << DEB_VAR1(aMode);
}

void Camera::_allocBuffer()
{
  DEB_MEMBER_FUNCT();

  uint32_t imageSize;
  ImageRaw info;
  int32_t error = arinax_ns::getImageInfo(m_handle, &info);
  imageSize = info.m_payloadSize;
  if(error)
    throw LIMA_HW_EXC(Error,"Can't get camera image size");

  DEB_TRACE() << DEB_VAR1(imageSize);
}
/** @brief start the acquisition.
    must have m_video != NULL and previously call _allocBuffer
*/
void Camera::startAcq()
{
  DEB_MEMBER_FUNCT();
  if(!m_continue_acq)
{
  m_continue_acq = true;
  m_acq_frame_nb = 0;
  int32_t error = arinax_ns::registerCallback(m_handle, &_newFrameCBK);
  arinax_ns::startGrabbing(m_handle);
  int requested_nb_frames;
  m_sync->getNbFrames(requested_nb_frames);
}
}

void Camera::stopAcq()
{
  DEB_MEMBER_FUNCT();
  arinax_ns::stopGrabbing(m_handle);
  m_continue_acq=false;
}

void Camera::getStatus(HwInterface::StatusType& status)
{
  DEB_MEMBER_FUNCT();
  if(m_continue_acq)
  {
     status.acq = AcqRunning;
     status.det = DetExposure;
  }     
  else    
  {
     status.acq = AcqReady;
     status.det = DetIdle;
  } 
  DEB_RETURN() << DEB_VAR1(status);
} 

void Camera::getZoomLevel(double& zl)
{
  DEB_MEMBER_FUNCT();
  double ret;
  int32_t error = arinax_ns::getZoomLevel(m_handle, &ret);
  zl = ret;
  DEB_RETURN() << DEB_VAR1(zl);
}

void Camera::setZoomLevel(double zl)
{
  DEB_MEMBER_FUNCT();
  int32_t error = arinax_ns::setZoomLevel(m_handle, zl);
  DEB_RETURN() << DEB_VAR1(zl);
}

void Camera::getZoom(unsigned int& zl)
{
  DEB_MEMBER_FUNCT();
  int32_t ret;
  int32_t error = arinax_ns::getZoom(m_handle, &ret);
  zl = ret;
  DEB_RETURN() << DEB_VAR1(zl);
}

void Camera::setZoom(unsigned int zl)
{
  DEB_MEMBER_FUNCT();
  int32_t error = arinax_ns::setZoom(m_handle, zl);
  DEB_RETURN() << DEB_VAR1(zl);
}

void Camera::getPredefinedZoomLevels(std::vector<double>& zls)
{
  DEB_MEMBER_FUNCT();
  std::lock_guard<std::mutex> lock(*m_predefZoomLock);
  m_predefZoomProm = std::make_shared<std::promise<std::vector<double>>>();
  std::future<std::vector<double>> res = m_predefZoomProm->get_future();
  int32_t error = arinax_ns::getPredefinedZoomLevel(m_handle);
  zls = res.get();
}

void Camera::getScale(double& scale)
{
  DEB_MEMBER_FUNCT();
  double ret;
  int32_t error = arinax_ns::getScale(m_handle, &ret);
  scale = ret;
  DEB_RETURN() << DEB_VAR1(scale);
}

void Camera::getScale(unsigned int idx, double& scale)
{
  DEB_MEMBER_FUNCT();
  double ret;
  int32_t error = arinax_ns::getZoomLevelScale(m_handle, idx, &ret);
  scale = ret;
  DEB_RETURN() << DEB_VAR1(scale);
}

void Camera::getZoomRange(std::vector<int>& zis)
{
  DEB_MEMBER_FUNCT();
  std::lock_guard<std::mutex> lock(*m_zoomRangeLock);
  m_zoomRangeProm = std::make_shared<std::promise<std::vector<int>>>();
  std::future<std::vector<int>> res = m_zoomRangeProm->get_future();
  int32_t error = arinax_ns::getZoomRange(m_handle);
  zis = res.get();
}

void Camera::reset()
{
  DEB_MEMBER_FUNCT();
  //@todo maybe something to do!
}

void Camera::_newFrameCBK(ImageRaw* aFrame)
{
  DEB_STATIC_FUNCT();
  if(a_camera)
  {
    a_camera->_newFrame(aFrame);
  }
}

void Camera::_newFrame(ImageRaw* aFrame)
{
  DEB_MEMBER_FUNCT();

  if(!m_continue_acq) return;

  int requested_nb_frames;
  m_sync->getNbFrames(requested_nb_frames);
  bool isLive;
  m_video->getLive(isLive);
  ++m_acq_frame_nb;

  VideoMode mode = RGB24;
  m_continue_acq =  m_video->callNewImage((char*)aFrame->m_Image,
					  aFrame->m_width,
					  aFrame->m_height,
					  mode);
  if(!m_continue_acq)
    stopAcq();
}

//-----------------------------------------------------
// @brief range the binning to the maximum allowed
//-----------------------------------------------------
void Camera::checkBin(Bin &hw_bin)
{
    DEB_MEMBER_FUNCT();


    int x = hw_bin.getX();
    if(x > m_maxwidth)
        x = m_maxwidth;

    int y = hw_bin.getY();
    if(y > m_maxheight)
        y = m_maxheight;

    hw_bin = Bin(x,y);
    DEB_RETURN() << DEB_VAR1(hw_bin);
}
//-----------------------------------------------------
// @brief set the new binning mode
//-----------------------------------------------------
void Camera::setBin(const Bin &set_bin)
{
    DEB_MEMBER_FUNCT();


    m_bin = set_bin;
    
    DEB_RETURN() << DEB_VAR1(set_bin);
}

//-----------------------------------------------------
// @brief return the current binning mode
//-----------------------------------------------------
void Camera::getBin(Bin &hw_bin)
{
    DEB_MEMBER_FUNCT(); 


    Bin tmp_bin(1, 1);
    
    hw_bin = tmp_bin;
    m_bin = tmp_bin;
    
    DEB_RETURN() << DEB_VAR1(hw_bin);
}

//-----------------------------------------------------
// @brief return the internal buffer manager
//-----------------------------------------------------
HwBufferCtrlObj* Camera::getBufferCtrlObj()
{
    DEB_MEMBER_FUNCT();
    return &m_buffer_ctrl_obj;
}

