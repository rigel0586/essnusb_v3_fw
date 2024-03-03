#pragma once

#ifndef EsbITrack_hpp
#define EsbITrack_hpp 1

#include <string>
#include <vector>
#include "TObject.h"
#include "TClonesArray.h"
#include <TVector3.h>

#include "ITrackPoint.hpp"

namespace esbroot {
namespace core {
namespace eve {

class ITrack : public TObject
{
    public:
        ITrack() = default;

        ITrack(ITrack& point) = default;
        ITrack& operator=(ITrack& point) = default;

        void addPoint(const ITrackPoint& point) {fPoints.emplace_back(point);}

        const std::vector<ITrackPoint>& getPoints(){
//            std::sort(fPoints.begin(), fPoints.end(), [](ITrackPoint& ls, ITrackPoint& rs){return ls.GetTime() < rs.GetTime();} );
            return fPoints;
        }

private:
    std::vector<ITrackPoint> fPoints;

    ClassDef(ITrack, 2);
};

} // namespace eve
} // namespace core
} // namespace esbroot

#endif
