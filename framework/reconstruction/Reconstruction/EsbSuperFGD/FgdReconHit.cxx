#include "EsbReconstruction/EsbSuperFGD/FgdReconHit.h"



namespace esbroot {
namespace reconstruction {
namespace superfgd {

ReconHit::ReconHit()
    :     fIsVisited(false)
        , fIsLeaf(false)
        , fZaxisAngle(0.)
        , fChangeAngle(0.)
        , fsmoothco(0,0,0)
{
}

ReconHit::ReconHit(TVector3 mppcLoc
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
            , Double_t pe)
        : fmppcLoc(mppcLoc)
        , fHitPos(hitPosition)
        , fMCPos(mCPos)
        , fphotons(photons)
        , ftime(time)
        , fmom(mom)
        , fmomExit(momExit)
        , ftrackLength(trackLength)
        , ftrackLengthOrigin(trackLengthOrigin)
        , fpdg(pdg)
        , ftrackId(trackId)
        , fEdep(edep)
        , fph1(ph1)
        , fmppc1(mppc1)
        , fph2(ph2)
        , fmppc2(mppc2)
        , fIsVisited(false)
        , fIsLeaf(false)
        , fXaxisAngle(0.)
        , fYaxisAngle(0.)
        , fZaxisAngle(0.)
        , fChangeAngle(0.)
        , fpe(pe)
        , fsmoothco(mppcLoc) // initially the smooth coordinates are the same as the cube`s location
{
    
}

ReconHit::~ReconHit()
{

}

ReconHit::ReconHit(const ReconHit& c)
        : fmppcLoc(c.fmppcLoc)
        , fHitPos(c.fHitPos)
        , fMCPos(c.fMCPos)
        , fphotons(c.fphotons)
        , fph1(c.fph1)
        , fmppc1(c.fmppc1)
        , fph2(c.fph2)
        , fmppc2(c.fmppc2)
        , ftime(c.ftime)
        , fmom(c.fmom)
        , fmomExit(c.fmomExit)
        , ftrackLength(c.ftrackLength)
        , ftrackLengthOrigin(c.ftrackLengthOrigin)
        , fpdg(c.fpdg)
        , ftrackId(c.ftrackId)
        , fEdep(c.fEdep)
        , fAllHits(c.fAllHits)
        , fIsVisited(c.fIsVisited)
        , fIsLeaf(c.fIsLeaf)
        , fXaxisAngle(c.fXaxisAngle)
        , fYaxisAngle(c.fYaxisAngle)
        , fZaxisAngle(c.fZaxisAngle)
        , fChangeAngle(c.fChangeAngle)
        , fpe(c.fpe)
        , fsmoothco(c.fsmoothco)
{
    
}

ReconHit& ReconHit::operator=(const ReconHit& c)
{
    fmppcLoc = c.fmppcLoc;
    fHitPos = c.fHitPos;
    fMCPos = c.fMCPos;
    fphotons = c.fphotons;
    fph1 = c.fph1;
    fmppc1 = c.fmppc1;
    fph2 = c.fph2;
    fmppc2 = c.fmppc2;
    ftime = c.ftime;
    fmom = c.fmom;
    fmomExit = c.fmomExit;
    ftrackLength = c.ftrackLength;
    ftrackLengthOrigin = c.ftrackLengthOrigin;
    fpdg = c.fpdg;
    ftrackId = c.ftrackId;
    fEdep = c.fEdep;
    fAllHits = c.fAllHits;
    fIsVisited = c.fIsVisited;
    fIsLeaf = c.fIsLeaf;
    fXaxisAngle = c.fXaxisAngle;
    fYaxisAngle = c.fYaxisAngle;
    fZaxisAngle = c.fZaxisAngle;
    fpe = c.fpe;
    fsmoothco = fsmoothco;
    return *this;
}

Bool_t ReconHit::operator==(const ReconHit& c)
{
    return (this->fHitPos.X() == c.fHitPos.X()
            && this->fHitPos.Y() == c.fHitPos.Y()
            && this->fHitPos.Z() == c.fHitPos.Z());
}


TVector3 ReconHit::operator-(const ReconHit& c)
{
    Int_t&& x = this->fmppcLoc.X()-c.fmppcLoc.X();
    Int_t&& y = this->fmppcLoc.Y()-c.fmppcLoc.Y();
    Int_t&& z = this->fmppcLoc.Z()-c.fmppcLoc.Z();
    return TVector3(x,y,z);
}

} //superfgd
} //reconstruction
} //esbroot