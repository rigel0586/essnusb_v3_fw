#include "generators/SuperFGD/FgdGeomAnalyzer.h"
ClassImp(esbroot::generators::superfgd::FgdGeomAnalyzer)

#include "generators/SuperFGD/GenieFluxDriver.h"
#include "geometry/SuperFGD/EsbSuperFGD/Names.h"

#include <TGeoNode.h>
#include <TObjArray.h>

#include <fairlogger/Logger.h>

#include "Framework/ParticleData/PDGCodeList.h"
#include "Framework/EventGen/GFluxI.h"

#include <sstream>
#include <iostream>

namespace esbroot {
namespace generators {
namespace superfgd {

FgdGeomAnalyzer::FgdGeomAnalyzer(TGeoManager* gm)
                : ROOTGeomAnalyzer(gm), fTopVol(nullptr)
{
    // 1. backup the top volume to reset it after use
    fTopVol = gm->GetTopVolume();

    // 2. Uncomment if you want to change lnegth units
    // ROOTGeomAnalyzer::SetLengthUnits(CLHEP::cm);

    // 3. Set the volume for which to generate events in
	ROOTGeomAnalyzer::SetTopVolName((esbroot::geometry::superfgd::fgdnames::superFGDName));
}


FgdGeomAnalyzer::~FgdGeomAnalyzer()
{
    Reset();
}

void FgdGeomAnalyzer::Reset()
{
    if(fTopVol!=nullptr)
    {
        //ROOTGeomAnalyzer::SetLengthUnits(genie::units::meter);
        ROOTGeomAnalyzer::SetTopVolName(fTopVol->GetName());
    }
}

const genie::PathLengthList & FgdGeomAnalyzer::ComputePathLengths(const TLorentzVector & x, const TLorentzVector & p)
{
    return  ROOTGeomAnalyzer::ComputePathLengths(x, p);
}

const TVector3 & FgdGeomAnalyzer::GenerateVertex(const TLorentzVector &x, const TLorentzVector &p, int tgtpdg)
{
    TVector3 geomVec = ROOTGeomAnalyzer::GenerateVertex(x, p, tgtpdg);
    // LOG(info) << "FgdGeomAnalyzer::GenerateVertex " << " x " << fposVec.x() 
    //                                             << " y " << fposVec.y() 
    //                                             << " z " << fposVec.z();

    // LOG(info) << "FgdGeomAnalyzer::GenerateVertex " << " x " << x.X() 
    //                                             << " y " << x.Y() 
    //                                             << " z " << x.Z();

    fposVec.SetXYZ( geomVec.X() * f_util.rootToG4CoeffLength()
                        , geomVec.Y() * f_util.rootToG4CoeffLength()
                        , geomVec.Z() * f_util.rootToG4CoeffLength());
    return fposVec;
}


} //namespace superfgd 
} //namespace generators
} //namespace esbroot