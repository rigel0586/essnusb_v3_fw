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
#include "G4Event.hh"
#include "G4Run.hh"

namespace esbroot {
namespace core {
namespace detector {

class IDetector : public TObject
{
    public:
        virtual ~IDetector() = default;

        virtual void ConstructGeometry() = 0;

        virtual void AddSensitiveDetector(G4VPhysicalVolume* topVolume 
                    , std::function<void(G4LogicalVolume*, G4VSensitiveDetector*)>& f_sd){}

        virtual void AddMultiSensitiveDetector(G4VPhysicalVolume* topVolume 
                    , std::function<void(std::string , G4VSensitiveDetector* , bool)>& f_sd_multi){}

        virtual void BeginOfEventAction(const G4Event*) {}
        virtual void EndOfEventAction(const G4Event*){}

        virtual void BeginOfRunAction(const G4Run* aRun) {}
        virtual void EndOfRunAction(const G4Run* aRun) {}

private:

    ClassDef(IDetector, 2);
};

} // namespace detector
} // namespace core
} // namespace esbroot

#endif
