#pragma once

#ifndef EsbDetectorConstructor_hpp
#define EsbDetectorConstructor_hpp 1


#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4SDManager.hh"

#include "IDetector.hpp"

namespace esbroot {
namespace esbcore {

class EsbDetectorConstructor : public G4VUserDetectorConstruction
{
public:
    EsbDetectorConstructor();
    ~EsbDetectorConstructor();

    // G4VUserDetectorConstruction
    G4VPhysicalVolume* Construct() override;

    void AddDetector(IDetector* d);

private:
    std::vector<IDetector*> fDetectors;
};

}
}
#endif
