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
#include <vector>

namespace esbroot {
namespace core {
namespace detector{

class EsbDetectorConstructor : public TObject, public G4VUserDetectorConstruction
{
public:
    EsbDetectorConstructor(const std::string& workDir, std::vector<IDetector*>& detectors);
    ~EsbDetectorConstructor();

    // G4VUserDetectorConstruction
    G4VPhysicalVolume* Construct() override;

    void AddDetector(IDetector* d);
    void setWorkingDir(const std::string& dirPath){fWorkDir = dirPath;}

    const std::string& getWorkingDir(){return fWorkDir;}

    
    void SetSensitiveHandler(G4LogicalVolume* logVol, G4VSensitiveDetector* aSD);
    void SetMultiSensitiveHandler(std::string logVolName, G4VSensitiveDetector* aSD, bool multi);

private:
    std::vector<IDetector*> fDetectors;
    
    std::string fWorkDir{""};
    const std::string fgdml{"esbVolume.gdml"};
    const std::string froot{"esbVolume.root"};

    const std::string fPostgdml{"esbInterVolume.gdml"};
    const std::string fFinalgdml{"esbFinalVolume.gdml"};
    const std::string fFinalroot{"esbFinalVolume.root"};
    
    io::EsbIO fIo;

    ClassDef(EsbDetectorConstructor, 2);
};

} // namespace detector
} // namespace core
} // namespace esbroot
#endif
