namespace Arinax
{
  class Interface : HwInterface
  {
%TypeHeaderCode
#include <ArinaxInterface.h>
%End
  public:
    Interface(Arinax::Camera* /KeepReference/);
    virtual ~Interface();

    virtual void getCapList(std::vector<HwCap> &cap_list /Out/) const;
    virtual void reset(ResetLevel reset_level);
    virtual void prepareAcq();
    virtual void startAcq();
    virtual void stopAcq();
    virtual void getStatus(StatusType& status /Out/);

    virtual int getNbAcquiredFrames();
    virtual int getNbHwAcquiredFrames();
  };

};
