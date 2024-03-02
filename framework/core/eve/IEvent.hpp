#pragma once

#ifndef EsbIEvent_hpp
#define EsbIEvent_hpp 1

#include <string>
#include "TObject.h"
#include "TClonesArray.h"
#include "ITrackPoint.hpp"

namespace esbroot {
namespace core {
namespace eve {

class IEvent: public TObject
{
    public:
        IEvent() {}
        
        void AddTrackPoint(ITrackPoint point){ /* TODO */}
        

private:

    ClassDef(IEvent, 2);
};

} // namespace eve
} // namespace core
} // namespace esbroot

#endif
