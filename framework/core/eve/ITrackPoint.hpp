#pragma once

#ifndef EsbITrackPoint_hpp
#define EsbITrackPoint_hpp 1

#include <string>
#include "TObject.h"
#include "TClonesArray.h"
#include <TVector3.h>

namespace esbroot {
namespace core {
namespace eve {

class ITrackPoint : public TObject
{
    public:
        ITrackPoint(Double_t time, Int_t pdg, Int_t trackId, TVector3 momentum, TVector3 position)
            : ftime(time)
                , fPdg(pdg)
                , ftrackId(trackId)
                , fmom(momentum)
                , fPosition(position)
        {}

        ITrackPoint(ITrackPoint& point) = default;
        ITrackPoint& operator=(ITrackPoint& point) = default;

        virtual ~ITrackPoint() = default;

        Double_t GetTime(){return ftime;}
        Int_t GetPgd(){return fPdg;}
        Int_t GetTrackId(){return ftrackId;}
        TVector3 GetMomentum(){return fmom;}
        TVector3 GetPosition() { return fPosition; };

private:
    Double_t ftime{0.};
    Int_t fPdg{0};   
    Int_t ftrackId{0};
    TVector3 fmom;
    TVector3 fPosition;

    ClassDef(ITrackPoint, 2);
};

} // namespace eve
} // namespace core
} // namespace esbroot

#endif
