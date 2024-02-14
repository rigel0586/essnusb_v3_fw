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
    fTopVol = gm->GetTopVolume();

    // ROOTGeomAnalyzer::SetLengthUnits(genie::units::centimeter);
    // ROOTGeomAnalyzer::SetLengthUnits(CLHEP::cm);
	ROOTGeomAnalyzer::SetTopVolName((esbroot::geometry::superfgd::fgdnames::superFGDName));
    // ROOTGeomAnalyzer::SetTopVolName(fTopVol->GetName());
    
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
    // fposVec = ROOTGeomAnalyzer::GenerateVertex(x, p, tgtpdg);
    // LOG(info) << "FgdGeomAnalyzer::GenerateVertex " << " x " << fposVec.x() 
    //                                             << " y " << fposVec.y() 
    //                                             << " z " << fposVec.z();

    // LOG(info) << "FgdGeomAnalyzer::GenerateVertex " << " x " << x.X() 
    //                                             << " y " << x.Y() 
    //                                             << " z " << x.Z();

    fposVec.SetXYZ( x.X() * f_util.rootToG4CoeffLength()
                        , x.Y() * f_util.rootToG4CoeffLength()
                        , x.Z() * f_util.rootToG4CoeffLength());
    return fposVec;
}


} //namespace superfgd 
} //namespace generators
} //namespace esbroot