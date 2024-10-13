#include "generators/generic/GenericGeomAnalyzer.h"
ClassImp(esbroot::generators::generic::GenericGeomAnalyzer)

#include "generators/generic/GenieFluxDriver.h"

#include <TGeoNode.h>
#include <TObjArray.h>

#include <fairlogger/Logger.h>

#include "Framework/ParticleData/PDGCodeList.h"
#include "Framework/EventGen/GFluxI.h"

#include <sstream>
#include <iostream>

namespace esbroot {
namespace generators {
namespace generic {

GenericGeomAnalyzer::GenericGeomAnalyzer(TGeoManager* gm, std::string geomName)
                : ROOTGeomAnalyzer(gm), fTopVol(nullptr), fgeomName(geomName)
{
    // 1. backup the top volume to reset it after use
    fTopVol = gm->GetTopVolume();

    // 2. Uncomment if you want to change lnegth units
    // ROOTGeomAnalyzer::SetLengthUnits(CLHEP::cm);

    // 3. Set the volume for which to generate events in
	ROOTGeomAnalyzer::SetTopVolName((fgeomName.c_str()));
}


GenericGeomAnalyzer::~GenericGeomAnalyzer()
{
    Reset();
}

void GenericGeomAnalyzer::Reset()
{
    if(fTopVol!=nullptr)
    {
        //ROOTGeomAnalyzer::SetLengthUnits(genie::units::meter);
        ROOTGeomAnalyzer::SetTopVolName(fTopVol->GetName());
    }
}

const genie::PathLengthList & GenericGeomAnalyzer::ComputePathLengths(const TLorentzVector & x, const TLorentzVector & p)
{
    return  ROOTGeomAnalyzer::ComputePathLengths(x, p);
}

const TVector3 & GenericGeomAnalyzer::GenerateVertex(const TLorentzVector &x, const TLorentzVector &p, int tgtpdg)
{
    TVector3 geomVec = ROOTGeomAnalyzer::GenerateVertex(x, p, tgtpdg);
    // LOG(info) << "GenericGeomAnalyzer::GenerateVertex " << " x " << fposVec.x() 
    //                                             << " y " << fposVec.y() 
    //                                             << " z " << fposVec.z();

    // LOG(info) << "GenericGeomAnalyzer::GenerateVertex " << " x " << x.X() 
    //                                             << " y " << x.Y() 
    //                                             << " z " << x.Z();

    fposVec.SetXYZ( geomVec.X() * f_util.rootToG4CoeffLength()
                        , geomVec.Y() * f_util.rootToG4CoeffLength()
                        , geomVec.Z() * f_util.rootToG4CoeffLength());
    return fposVec;
}


} //namespace generic 
} //namespace generators
} //namespace esbroot