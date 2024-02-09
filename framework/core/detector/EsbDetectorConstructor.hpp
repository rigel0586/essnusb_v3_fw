#pragma once

#ifndef EsbDetectorConstructor_hpp
#define EsbDetectorConstructor_hpp 1

#include "TObject.h"

#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4SDManager.hh"

#include "EsbIO.hpp"
#include "IDetector.hpp"

#include <string>

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

    void setWorkingDir(const std::string& dirPath){fWorkDir = dirPath;}
    void setGdmlFile(const std::string& fileName){fgdml = fileName;};

    void SetSensitiveHandler(G4LogicalVolume* logVol, G4VSensitiveDetector* aSD);

private:
    std::vector<IDetector*> fDetectors;
    
    std::string fWorkDir{""};
    std::string fgdml{"esbVolume.gdml"};

    io::EsbIO fIo;

    ClassDef(EsbDetectorConstructor, 2);
};

} // namespace detector
} // namespace core
} // namespace esbroot
#endif
