#pragma once

#ifndef EsbISDetector_hpp
#define EsbISDetector_hpp 1

#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"

#include "TGeoVolume.h"
#include "TObject.h"
#include <string>

namespace esbroot {
namespace core {
namespace detector {

// Sensitive detector interface
class ISDetector : public TObject
{
    public:
        virtual ~ISDetector() = default;

        virtual void AddSensitiveDetector(G4VPhysicalVolume* topVolume) = 0;

private:

    ClassDef(ISDetector, 2);
};

} // namespace detector
} // namespace core
} // namespace esbroot

#endif
