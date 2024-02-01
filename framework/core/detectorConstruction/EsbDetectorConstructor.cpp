#include "EsbDetectorConstructor.hpp"

namespace esbroot {
namespace esbcore {

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
        // TODO
    }
}

void EsbDetectorConstructor::AddDetector(IDetector* d)
{
    if(d != nullptr)
        fDetectors.emplace_back(d):
}

}
}

