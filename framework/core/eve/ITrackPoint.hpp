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
        ITrackPoint() {};

        ITrackPoint(Double_t time, Int_t pdg, Int_t trackId, TVector3 momentum, TVector3 position)
            : ftime(time)
                , fPdg(pdg)
                , ftrackId(trackId)
                , fmom(momentum)
                , fPosition(position)
        {}

        ITrackPoint(const ITrackPoint& point){
            ftime = point.GetTime();
            fPdg = point.GetPgd();
            ftrackId = point.GetTrackId();
            fmom = point.GetMomentum();
            fPosition = point.GetPosition();
        }

        ITrackPoint(ITrackPoint&& point){
            ftime = point.GetTime();
            fPdg = point.GetPgd();
            ftrackId = point.GetTrackId();
            fmom = point.GetMomentum();
            fPosition = point.GetPosition();
        }

        ITrackPoint& operator=(const ITrackPoint& point){
            ftime = point.GetTime();
            fPdg = point.GetPgd();
            ftrackId = point.GetTrackId();
            fmom = point.GetMomentum();
            fPosition = point.GetPosition();

            return *this;
        }

        ITrackPoint& operator=(ITrackPoint&& point){
            ftime = point.GetTime();
            fPdg = point.GetPgd();
            ftrackId = point.GetTrackId();
            fmom = point.GetMomentum();
            fPosition = point.GetPosition();

            return *this;
        }

        virtual ~ITrackPoint() = default;

        Double_t GetTime(){return ftime;}
        Int_t GetPgd(){return fPdg;}
        Int_t GetTrackId(){return ftrackId;}
        TVector3 GetMomentum(){return fmom;}
        TVector3 GetPosition() { return fPosition; };

        Double_t GetTime() const {return ftime;}
        Int_t GetPgd() const {return fPdg;}
        Int_t GetTrackId() const {return ftrackId;}
        TVector3 GetMomentum() const {return fmom;}
        TVector3 GetPosition() const { return fPosition; };

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
