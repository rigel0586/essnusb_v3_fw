#ifndef ESBROOT_ESBDRECONSTRUCTION_FGD_RECONHIT_H
#define ESBROOT_ESBDRECONSTRUCTION_FGD_RECONHIT_H

#include "TObject.h"
#include <TVector3.h>
#include <vector>
#include <algorithm>

namespace esbroot {
namespace reconstruction {
namespace superfgd {

class ReconHit : public TObject
{

public:

    /** Default constructor **/  
    ReconHit();

    ReconHit(TVector3 mppcLoc
            , TVector3 hitPosition
            , TVector3 mCPos
            , TVector3 photons
            , Double_t time
            , TVector3 mom
            , TVector3 momExit
            , Double_t trackLength
            , Double_t trackLengthOrigin
            , Int_t pdg
            , Int_t trackId
            , Double_t edep
            , TVector3 ph1
            , TVector3 mppc1
            , TVector3 ph2
            , TVector3 mppc2
            , Double_t pe);

    ~ReconHit();

    ReconHit(const ReconHit& c);

    ReconHit& operator=(const ReconHit& c);

    Bool_t operator==(const ReconHit& c);

    TVector3 operator-(const ReconHit& c);

    Bool_t IsAlone()
    {
        return fAllHits.empty(); 
    } 


    TVector3 getSmoothErr()
    {
        Double_t&& x = fmppcLoc.X() - fsmoothco.X();
        Double_t&& y = fmppcLoc.Y() - fsmoothco.Y();
        Double_t&& z = fmppcLoc.Z() - fsmoothco.Z();
        return TVector3(x,y,z);
    }

    TVector3 fmppcLoc; // from 0 to N (mppcLoc)
    TVector3 fHitPos;  // -f_total_X/2 + f_step_X*mppcLoc.X() + f_step_X/2;
    TVector3 fMCPos;   // MC real position of the hit;
    TVector3 fphotons;
    TVector3 fph1;
    TVector3 fmppc1;
    TVector3 fph2;
    TVector3 fmppc2;
    Double_t ftime;
    TVector3 fmom;
    TVector3 fmomExit;
    Double_t ftrackLength;       // Lenght used by summing on each step (it is less than ftrackLengthOrigin)
    Double_t ftrackLengthOrigin; // TVirtualMC::GetMC()->TrackLength() -> Monte Carlo length
    Double_t fEdep;
    Int_t fpdg;
    Int_t ftrackId;
    Bool_t fIsLeaf;

    TVector3 fsmoothco;

    Double_t fXaxisAngle;
    Double_t fYaxisAngle;
    Double_t fZaxisAngle;
    Double_t fChangeAngle;
    Double_t fpe;

    Int_t fLocalId;     // Id which is used when building a graph - relative to the graph structure
    Long_t fHitId;      // Id which is used in detector coordinates - relative to the position in the detector
    Bool_t fIsVisited;
    std::vector<ReconHit*> fAllHits;//!<! 

private:

    ClassDef(ReconHit, 2);
};

} //superfgd
} //reconstruction
} //esbroot

#endif