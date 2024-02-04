#pragma once

#ifndef EsbDetectorConstructor_hpp
#define EsbDetectorConstructor_hpp 1

#include "TObject.h"

#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4SDManager.hh"

#include "IDetector.hpp"

namespace esbroot {
namespace core {
namespace detector{

class EsbDetectorConstructor : public TObject, public G4VUserDetectorConstruction
{
public:
    EsbDetectorConstructor();
    ~EsbDetectorConstructor();

    // G4VUserDetectorConstruction
    G4VPhysicalVolume* Construct() override;

    void AddDetector(IDetector* d);

private:
    std::vector<IDetector*> fDetectors;

    ClassDef(EsbDetectorConstructor, 2);
};

} // namespace detector
} // namespace core
} // namespace esbroot
#endif
