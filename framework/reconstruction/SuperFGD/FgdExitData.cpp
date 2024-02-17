#include "reconstruction/SuperFGD/FgdExitData.hpp"
ClassImp(esbroot::reconstruction::superfgd::FgdExitData)



namespace esbroot {
namespace reconstruction {
namespace superfgd {

FgdExitData::FgdExitData()
{
}

FgdExitData::FgdExitData(Int_t nuPdg,
                Double_t nuE,
                Bool_t IsWeakCC,
                Bool_t IsWeakNC,
                Bool_t IsQuasiElastic,
                std::vector<FgdExitParticle> vecPars)
        : fnuPdg(nuPdg)
        , fnuE(nuE)
        , fIsWeakCC(IsWeakCC)
        , fIsWeakNC(IsWeakNC)
        , fIsQuasiElastic(IsQuasiElastic)
        , fVecPars(vecPars)
        , fHasExiting(false)
        , fIsPrimaryMuon(false)
        , fIsPrimaryElectron(false)
        , fTotalPhotons(0,0,0)
        , fTotalCubes(0)
        , fMaxtrack(0.)
        , fnumOfTracks(0)
        , fPrimaryLeptonMom(0,0,0)
        , fmean(0.)
        , fstdDev(0.)
        , fmaxVal(0.)
        , fEventNum(0)
{
    
}

FgdExitData::~FgdExitData()
{

}

FgdExitData::FgdExitData(const FgdExitData& c)
        : fnuPdg(c.fnuPdg)
        , fnuE(c.fnuE)
        , fIsWeakCC(c.fIsWeakCC)
        , fIsWeakNC(c.fIsWeakNC)
        , fIsQuasiElastic(c.fIsQuasiElastic)
        , fVecPars(c.fVecPars)
        , fHasExiting(c.fHasExiting)
        , fIsPrimaryMuon(c.fIsPrimaryMuon)
        , fIsPrimaryElectron(c.fIsPrimaryElectron)
        , fTotalPhotons(c.fTotalPhotons)
        , fTotalCubes(c.fTotalCubes)
        , fMaxtrack(c.fMaxtrack)
        , fnumOfTracks(c.fnumOfTracks)
        , fPrimaryLeptonMom(c.fPrimaryLeptonMom)
        , fmean(c.fmean)
        , fstdDev(c.fstdDev)
        , fmaxVal(c.fmaxVal)
        , fEventNum(c.fEventNum)
{
}

FgdExitData& FgdExitData::operator=(const FgdExitData& c)
{
    fnuPdg = c.fnuPdg;
    fnuE = c.fnuE;
    fIsWeakCC = c.fIsWeakCC;
    fIsWeakNC = c.fIsWeakNC;
    fIsQuasiElastic = c.fIsQuasiElastic;
    fVecPars = c.fVecPars;
    fHasExiting = c.fHasExiting;
    fIsPrimaryMuon = c.fIsPrimaryMuon;
    fIsPrimaryElectron = c.fIsPrimaryElectron;
    fTotalPhotons = c.fTotalPhotons;
    fTotalCubes = c.fTotalCubes;
    fMaxtrack = c.fMaxtrack;
    fnumOfTracks = c.fnumOfTracks;
    fPrimaryLeptonMom = c.fPrimaryLeptonMom;
    fmean = c.fmean;
    fstdDev = c.fstdDev;
    fmaxVal = c.fmaxVal;
    fEventNum = c.fEventNum;
    return *this;
}

} //superfgd
} //reconstruction
} //esbroot