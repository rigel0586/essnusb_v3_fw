#include "EsbDetectorConstructor.hpp"
ClassImp(esbroot::core::detector::EsbDetectorConstructor)

namespace esbroot {
namespace core {
namespace detector {

EsbDetectorConstructor::EsbDetectorConstructor(const std::string& workDir, std::vector<IDetector*>& detectors)
    : fWorkDir(workDir), fDetectors(detectors)
{
}

EsbDetectorConstructor::~EsbDetectorConstructor()
{
}

G4VPhysicalVolume* EsbDetectorConstructor::Construct()
{
    using namespace std::placeholders;

    for(IDetector* d : fDetectors)
    {
        d->ConstructGeometry(); // Every detector constructs its volumes and add it to the Cern root Top volume
    }

    std::string fileGdml = fWorkDir + "/" + fgdml;
    fIo.ExportTGeoVolume(fileGdml);
    std::string fileRoot = fWorkDir + "/" + froot;
    fIo.ExportTGeoVolume(fileRoot);

    G4VPhysicalVolume* convertedWorld = fIo.readGdmlToGeant4(fileGdml);

    for(IDetector* d : fDetectors)
    {
        d->PostConstructG4Geometry(convertedWorld); // Pass G4 world if any post convertion configuration is required
    }

    std::function<void(G4LogicalVolume*, G4VSensitiveDetector*)> f_sd = 
                    std::bind(&EsbDetectorConstructor::SetSensitiveHandler, this, _1, _2);

    std::function<void(std::string, G4VSensitiveDetector*, bool)> f_sd_multi = 
                    std::bind(&EsbDetectorConstructor::SetMultiSensitiveHandler, this, _1, _2, _3);

    for(IDetector* sd : fDetectors)
    {
        sd->AddSensitiveDetector(convertedWorld, f_sd); // Every detector should find its sensitive volume
    }

    for(IDetector* sd : fDetectors)
    {
        sd->AddMultiSensitiveDetector(convertedWorld, f_sd_multi); // Every detector should find its sensitive volume
    }

    return convertedWorld;
}

void EsbDetectorConstructor::SetSensitiveHandler(G4LogicalVolume* logVol, G4VSensitiveDetector* aSD)
{
    if(G4SDManager::GetSDMpointer()->FindSensitiveDetector(aSD->GetName()) == nullptr){
        G4SDManager::GetSDMpointer()->AddNewDetector(aSD);
    }

    if(logVol->GetSensitiveDetector() == nullptr){
        G4VUserDetectorConstruction::SetSensitiveDetector(logVol, aSD);
    }
}

void EsbDetectorConstructor::SetMultiSensitiveHandler(std::string logVolName, G4VSensitiveDetector* aSD, bool multi)
{
    if(G4SDManager::GetSDMpointer()->FindSensitiveDetector(aSD->GetName()) == nullptr){
        G4SDManager::GetSDMpointer()->AddNewDetector(aSD);
    }
    G4VUserDetectorConstruction::SetSensitiveDetector(logVolName, aSD, multi);
}
    

void EsbDetectorConstructor::AddDetector(IDetector* d)
{
    if(d != nullptr)
        fDetectors.emplace_back(d);
}

} // namespace detector
} // namespace core
} // namespace esbroot

