#ifndef ESBROOT_ESBEVEEVENT_EMULSION_DIGITIZER_H
#define ESBROOT_ESBEVEEVENT_EMULSION_DIGITIZER_H

#include <vector>

#include <TClonesArray.h>
#include "TRandom3.h"

#include "core/eve/IEvent.hpp"
#include "core/eve/ITrack.hpp"
#include "core/eve/ITrackPoint.hpp"

namespace esbroot {
namespace eve {
namespace emulsion {


class EveEvent : public core::eve::IEvent 
{

 public:

  EveEvent();

  /** Destructor **/
  ~EveEvent();


  /** Virtual method Exec **/
  virtual bool Exec(int eventId, TClonesArray* data, std::vector<core::eve::ITrack>& tracks) override;


private:
    /** Input array of EmulsionDataPoint(s)**/
    TClonesArray* fdPoints;     //! 
      	   
    ClassDef(EveEvent, 2);
};

} //emulsion
} //eve
} //esbroot

#endif // ESBROOT_ESBEVEEVENT_EMULSION_DIGITIZER_H
