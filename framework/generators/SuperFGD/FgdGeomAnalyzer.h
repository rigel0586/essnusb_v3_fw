#ifndef ESBROOT_GENERATOR_FGD_ROOTGEOM_ANALYZER_H
#define ESBROOT_GENERATOR_FGD_ROOTGEOM_ANALYZER_H 1

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

#include "geometry/SuperFGD/EsbSuperFGD/FgdDetectorParameters.h"

namespace esbroot {
namespace generators {
namespace superfgd {

using namespace std;

// Use ROOTGeomAnalyzer since it has implemented the methods
// overwrite only the ComputeMaxPathLength method
class FgdGeomAnalyzer : public genie::geometry::ROOTGeomAnalyzer
                    ,   public TObject  {

public:

    FgdGeomAnalyzer(TGeoManager* gm);
    ~FgdGeomAnalyzer();

    virtual const genie::PathLengthList& ComputePathLengths(const TLorentzVector & x, const TLorentzVector & p) override;

    virtual const TVector3 & GenerateVertex (const TLorentzVector &x, const TLorentzVector &p, int tgtpdg) override;

    void Reset();

private:
    TGeoVolume* fTopVol;
    TVector3 fposVec;

    utility::Utility f_util;

    ClassDef(FgdGeomAnalyzer,6)
};

} //namespace superfgd 
} //namespace generators
} //namespace esbroot

#endif
