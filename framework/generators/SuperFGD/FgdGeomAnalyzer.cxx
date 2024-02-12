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
	ROOTGeomAnalyzer::SetTopVolName((esbroot::geometry::superfgd::fgdnames::superFGDName));
    // ROOTGeomAnalyzer::SetTopVolName("caveVol");
    
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


} //namespace superfgd 
} //namespace generators
} //namespace esbroot