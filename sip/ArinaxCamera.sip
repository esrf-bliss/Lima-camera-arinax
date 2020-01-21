namespace Arinax
{
  class Camera
  {
%TypeHeaderCode
#include <ArinaxCamera.h>
%End
  public:
    Camera(const std::string& ip_addr,bool=true, bool mono_forced = false);
    ~Camera();
      
    bool isMonochrome() const;
    void getMaxWidthHeight(unsigned int& width /Out/,unsigned int& height /Out/);
    int getNbAcquiredFrames() const;

    void checkBin(Bin& /In,Out/);
    void setBin(const Bin&);
    void getBin(Bin& /Out/);

    VideoMode getVideoMode() const;
    void 	setVideoMode(VideoMode);
      
    void 	startAcq();
    void 	stopAcq();
    void        getZoomLevel(double& zl /Out/);
    void        setZoomLevel(double zl);
    void        getZoom(unsigned int& zoom /Out/);
    void        setZoom(unsigned int zoom);
    void        getZoomRange(std::vector<int>& zis /Out/);
    void        getPredefinedZoomLevels(std::vector<double>& zls /Out/);
    void        getScale(double& scale /Out/);
    void        getScale(int idx /In/, double& scale /Out/);
    void	reset();
  };
};
