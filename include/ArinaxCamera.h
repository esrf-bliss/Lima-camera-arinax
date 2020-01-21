#ifndef ARINAXCAMERA_H
#define ARINAXCAMERA_H
#include "Arinax.h"
#include "lima/Debug.h"
#include "lima/Constants.h"
#include "lima/HwMaxImageSizeCallback.h"
#include "lima/HwBufferMgr.h"
#include "lima/HwInterface.h"
#include <mutex>
#include <future>

namespace lima
{
  namespace Arinax
  {
    using namespace arinax_ns;
    class SyncCtrlObj;
    class VideoCtrlObj;
    class Camera : public HwMaxImageSizeCallbackGen
    {
      friend class Interface;
      friend class VideoCtrlObj;
      DEB_CLASS_NAMESPC(DebModCamera,"Camera","Arinax");
    public:
      Camera(const std::string& name,bool master = true, bool mono_forced = false);
      ~Camera();
      
      bool isMonochrome() const;
      int32_t getHandle() {return m_handle;}
      void getMaxWidthHeight(unsigned int& width,unsigned int& height)
      {width = m_maxwidth, height = m_maxheight;}
      int getNbAcquiredFrames() const {return m_acq_frame_nb;}

      VideoMode getVideoMode() const;
      void 	setVideoMode(VideoMode);
      
      void checkBin(Bin&);
      void setBin(const Bin&);
      void getBin(Bin&);

      void	getCameraName(std::string& name);
	
      void 	startAcq();
      void 	stopAcq();
      void 	getStatus(HwInterface::StatusType& );

      void      getZoomLevel(double& zl);
      void      setZoomLevel(double zl);
      void      getZoom(unsigned int& zl);
      void      setZoom(unsigned int zl);
      void      getPredefinedZoomLevels(std::vector<double>& zls);
      void      getZoomRange(std::vector<int>& zis);
      void      getScale(double& scale);
      void      getScale(unsigned int idx, double& scale);

      void	reset();
      HwBufferCtrlObj* getBufferCtrlObj();     

      bool		m_as_master;

    private:
      static Camera*    a_camera;
      static void       _camerasNameCBK(int32_t idx, const char * name){if(a_camera){a_camera->_camerasName(idx, name);}};
      void              _camerasName(int32_t, const char *);
      static void       _infoCBK(int32_t idx, const char * info){if(a_camera){a_camera->_info(idx, info);}};
      void              _info(int32_t, const char *);
      static void       _doubleParamCBK(const double val, const char * param){if(a_camera){a_camera->_doubleParam(val, param);}};
      void              _doubleParam(const double, const char *);
      static void       _intParamCBK(const char * param, const int64_t val){if(a_camera){a_camera->_intParam(param, val);}};
      void              _intParam(const char *, const int64_t);
      static void       _stringParamCBK(const char *param, const char* val){if(a_camera){a_camera->_stringParam(param, val);}};
      void              _stringParam(const char *, const char*);
      static void       _doubleRangeParamCBK(const char * param, int32_t nVals, double* vals){if(a_camera){a_camera->_doubleRangeParam(param, nVals, vals);}};
      void              _doubleRangeParam(const char *, int32_t, double*);
      static void       _intRangeParamCBK(const char * param, int32_t nVals, int64_t* vals){if(a_camera){a_camera->_intRangeParam(param, nVals, vals);}};
      void              _intRangeParam(const char *, int32_t, int64_t*);
      static void       _stringRangeParamCBK(const char * param, int32_t nVals, const char** vals){if(a_camera){a_camera->_stringRangeParam(param, nVals, vals);}};
      void              _stringRangeParam(const char *, int32_t, const char**);
      static void       _zoomLevelRangeCBK(int32_t nVals, double* vals){if(a_camera){a_camera->_zoomLevelRange(nVals, vals);}};
      void              _zoomLevelRange(int32_t, double*);
      static void       _zoomRangeCBK(int32_t nVals, int32_t* vals){if(a_camera){a_camera->_zoomRange(nVals, vals);}};
      void              _zoomRange(int32_t, int32_t*);
      static void       _predefZoomCBK(int32_t nVals, double* vals){if(a_camera){a_camera->_predefZoom(nVals, vals);}};
      void              _predefZoom(int32_t, double*);
      std::shared_ptr<std::mutex>        m_predefZoomLock;
      std::shared_ptr<std::promise<std::vector<double>>> m_predefZoomProm;
      std::shared_ptr<std::mutex>        m_zoomRangeLock;
      std::shared_ptr<std::promise<std::vector<int>>> m_zoomRangeProm;
      static void       _expoRangeCBK(int32_t nVals, double* vals){if(a_camera){a_camera->_expoRange(nVals, vals);}};
      void              _expoRange(int32_t, double*);
      static void       _gainRangeCBK(int32_t nVals, double* vals){if(a_camera){a_camera->_gainRange(nVals, vals);}};
      void              _gainRange(int32_t, double*);
      static void       _wbRangeCBK(const char * color, int32_t nVals, double* vals){if(a_camera){a_camera->_wbRange(color, nVals, vals);}};
      void              _wbRange(const char *,int32_t, double*);
      void 		_allocBuffer();
      static void 	_newFrameCBK(ImageRaw*);
      void		_newFrame(ImageRaw*);

      bool 		m_cam_connected;
      int32_t		m_handle;
      char		m_camera_name[128];
      char		m_sensor_type[64];
      uint32_t		m_ufirmware_maj, m_ufirmware_min;
      uint32_t		m_maxwidth, m_maxheight;
      uint32_t		m_uid;
      ImageRaw		m_frame[2];
      Bin               m_bin;
      
      SyncCtrlObj*	m_sync;
      VideoCtrlObj*	m_video;
      VideoMode		m_video_mode;
      int		m_acq_frame_nb;
      bool		m_continue_acq;
      bool              m_mono_forced;
      SoftBufferCtrlObj           m_buffer_ctrl_obj;
    };
  }
}
#endif
