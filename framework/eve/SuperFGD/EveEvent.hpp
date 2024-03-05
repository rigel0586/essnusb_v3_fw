#ifndef ESBROOT_ESBEVEEVENT_FGD_DIGITIZER_H
#define ESBROOT_ESBEVEEVENT_FGD_DIGITIZER_H

#include <vector>

#include <TClonesArray.h>
#include "TRandom3.h"

#include "geometry/SuperFGD/EsbSuperFGD/FgdDetectorParameters.h"

#include "core/eve/IEvent.hpp"
#include "core/eve/ITrack.hpp"
#include "core/eve/ITrackPoint.hpp"

namespace esbroot {
namespace eve {
namespace superfgd {


class EveEvent : public core::eve::IEvent 
{

 public:

  EveEvent();

  EveEvent(const char* geoConfigFile);

  /** Destructor **/
  ~EveEvent();


  /** Virtual method Exec **/
  virtual bool Exec(int eventId, TClonesArray* data, std::vector<core::eve::ITrack>& tracks) override;


private:
    /** Class to hold the Detector parameters read from external file **/
    esbroot::geometry::superfgd::FgdDetectorParameters fParams;

    /** Input array of FgdDetectorPoint(s)**/
    TClonesArray* fdPoints;     //! 

    std::vector<core::eve::ITrackPoint> fAllPoints; //! 
      	   
    ClassDef(EveEvent, 2);
};

} //superfgd
} //eve
} //esbroot

#endif // ESBROOT_ESBEVEEVENT_FGD_DIGITIZER_H
