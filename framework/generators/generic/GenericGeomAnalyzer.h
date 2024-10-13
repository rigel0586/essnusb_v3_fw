#ifndef ESBROOT_GENERATOR_GENERIC_ROOTGEOM_ANALYZER_H
#define ESBROOT_GENERATOR_GENERIC_ROOTGEOM_ANALYZER_H 1

#include "CLHEP/Units/SystemOfUnits.h"
#include "CLHEP/Units/PhysicalConstants.h"

#include "utility/Utility.hpp"

#include <Tools/Geometry/ROOTGeomAnalyzer.h>
#include <Framework/EventGen/PathLengthList.h>
#include <Framework/Conventions/Units.h>

#include "TGeoManager.h"
#include <TGeoVolume.h>
#include <TVector3.h>
#include <TLorentzVector.h>

namespace esbroot {
namespace generators {
namespace generic {

using namespace std;

// Use ROOTGeomAnalyzer since it has implemented the methods
// overwrite only the ComputeMaxPathLength method
class GenericGeomAnalyzer : public genie::geometry::ROOTGeomAnalyzer
                    ,   public TObject  {

public:

    GenericGeomAnalyzer(TGeoManager* gm, std::string geomName);
    ~GenericGeomAnalyzer();

    virtual const genie::PathLengthList& ComputePathLengths(const TLorentzVector & x, const TLorentzVector & p) override;

    virtual const TVector3 & GenerateVertex (const TLorentzVector &x, const TLorentzVector &p, int tgtpdg) override;

    void Reset();

private:
    TGeoVolume* fTopVol;
    TVector3 fposVec;
    const std::string fgeomName;

    utility::Utility f_util;

    ClassDef(GenericGeomAnalyzer,6)
};

} //namespace generic 
} //namespace generators
} //namespace esbroot

#endif
