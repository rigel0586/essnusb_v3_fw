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
#include <tuple>

namespace esbroot {
namespace core {
namespace detector{

enum class GeomConverter : int
{
    G4Root = 0,
    VGM = 1
};

class EsbDetectorConstructor : public TObject, public G4VUserDetectorConstruction
{
public:
    EsbDetectorConstructor(const std::string& workDir, std::vector<IDetector*>& detectors , GeomConverter converter = GeomConverter::G4Root);
    EsbDetectorConstructor(const std::string& view_File);
    ~EsbDetectorConstructor();

    // G4VUserDetectorConstruction
    G4VPhysicalVolume* Construct() override;
    G4VPhysicalVolume* Create();

    void AddDetector(IDetector* d);
    
    const std::string& getWorkingDir(){return fWorkDir;}

    void setWorkingDir(const std::string& dirPath){fWorkDir = dirPath;}
    void isView(bool isView, bool checkOverlap = false){f_isView = isView; fOverlapCheck = checkOverlap;}
    void SetSensitiveHandler(G4LogicalVolume* logVol, G4VSensitiveDetector* aSD);
    void SetMultiSensitiveHandler(std::string logVolName, G4VSensitiveDetector* aSD, bool multi);

private:
    std::vector<IDetector*> fDetectors;
    
    std::string fWorkDir{""};
    const std::string fgdml{"esbVolume.gdml"};
    const std::string froot{"esbVolume.root"};

    GeomConverter fConverter;

    const std::string fPostgdml{"esbInterVolume.gdml"};
    const std::string fPostVgmRoot{"esbInterVolume.root"};
    const std::string fFinalgdml{"esbFinalVolume.gdml"};
    const std::string fFinalroot{"esbFinalVolume.root"};

    std::string f_dgml_view_File{""};
    bool f_isView{false};
    bool fOverlapCheck{false};
    
    io::EsbIO fIo;

    std::vector< std::tuple< G4VPhysicalVolume*, G4VSolid*>>  CheckOverlap(G4VPhysicalVolume* volume
                    , G4int res = 1000
                    , G4double tol = 0.0
                    , G4bool verbose = true
                    , G4int errMax = 1);
    void DrawOverlap(std::vector< std::tuple< G4VPhysicalVolume*, G4VSolid*>>  Overlaps); 
    double AddTransparency(G4VPhysicalVolume* volume, G4double alpha);

    ClassDef(EsbDetectorConstructor, 2);
};

} // namespace detector
} // namespace core
} // namespace esbroot
#endif
