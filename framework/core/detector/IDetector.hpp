#pragma once

#ifndef EsbIDetector_hpp
#define EsbIDetector_hpp 1

#include <string>
#include <functional>

#include "TGeoVolume.h"
#include "TObject.h"

#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4VSensitiveDetector.hh"

namespace esbroot {
namespace core {
namespace detector {

class IDetector : public TObject
{
    public:
        virtual ~IDetector() = default;

        virtual void ConstructGeometry() = 0;

        virtual void AddSensitiveDetector(G4VPhysicalVolume* topVolume 
                                            , std::function<void(G4LogicalVolume*, G4VSensitiveDetector*)>& f_sd) = 0;

private:

    ClassDef(IDetector, 2);
};

} // namespace detector
} // namespace core
} // namespace esbroot

#endif
