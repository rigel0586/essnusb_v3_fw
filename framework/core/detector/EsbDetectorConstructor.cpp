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
    for(IDetector* d : fDetectors)
    {
        d->ConstructGeometry(); // Every detector constructs its volumes and add it to the Top volume
    }

    std::string fileGdml = fWorkDir + "/" + fgdml;
    fIo.ExportTGeoVolume(fileGdml);

    G4VPhysicalVolume* convertedWorld = fIo.readGdmlToGeant4(fileGdml);

    for(ISDetector* sd : fSDetectors)
    {
        sd->AddSensitiveDetector(convertedWorld); // Every detector should find its sensitive volume
    }

    return convertedWorld;
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

