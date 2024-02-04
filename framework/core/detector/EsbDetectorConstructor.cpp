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

    // TODO add export import from GDML
    return nullptr;
}

void EsbDetectorConstructor::AddDetector(IDetector* d)
{
    if(d != nullptr)
        fDetectors.emplace_back(d);
}

} // namespace detector
} // namespace core
} // namespace esbroot

