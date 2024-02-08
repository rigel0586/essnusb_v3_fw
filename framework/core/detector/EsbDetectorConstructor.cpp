#include "EsbDetectorConstructor.hpp"
ClassImp(esbroot::core::detector::EsbDetectorConstructor)

namespace esbroot {
namespace core {
namespace detector {

EsbDetectorConstructor::EsbDetectorConstructor()
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
        d->ConstructGeometry(); // Every detector constructs its volumes and add it to the Top volume
    }

    std::string fileGdml = fWorkDir + "/" + fgdml;
    fIo.ExportTGeoVolume(fileGdml);

    G4VPhysicalVolume* convertedWorld = fIo.readGdmlToGeant4(fileGdml);

    std::function<void(G4LogicalVolume*, G4VSensitiveDetector*)> f_sd = 
                    std::bind(&EsbDetectorConstructor::SetSensitiveHandler, this, _1, _2);

    for(ISDetector* sd : fSDetectors)
    {
        sd->AddSensitiveDetector(convertedWorld, f_sd); // Every detector should find its sensitive volume
    }

    return convertedWorld;
}

void EsbDetectorConstructor::SetSensitiveHandler(G4LogicalVolume* logVol, G4VSensitiveDetector* aSD)
{
    G4SDManager::GetSDMpointer()->AddNewDetector(aSD);
    G4VUserDetectorConstruction::SetSensitiveDetector(logVol, aSD);
}
    

void EsbDetectorConstructor::AddDetector(IDetector* d)
{
    if(d != nullptr)
        fDetectors.emplace_back(d);
}

void EsbDetectorConstructor::AddSDetector(ISDetector* sd)
{
    if(sd != nullptr)
        fSDetectors.emplace_back(sd);
}

} // namespace detector
} // namespace core
} // namespace esbroot

