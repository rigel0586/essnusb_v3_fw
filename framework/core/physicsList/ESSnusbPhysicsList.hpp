#pragma once

#ifndef ESSnusbPhysicsList_hpp
#define ESSnusbPhysicsList_hpp 1

#include "G4ios.hh"
//#include "iomanip.h"
#include "globals.hh"

#include "G4VModularPhysicsList.hh"
#include "G4VPhysicsConstructor.hh"

#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4LossTableManager.hh"

#include "TObject.h"

namespace esbroot {
namespace core {
namespace physicsList {

class ESSnusbPhysicsList : public TObject, public G4VModularPhysicsList {
public:
    ESSnusbPhysicsList(std::vector<G4VPhysicsConstructor*>& customProcesses);
    virtual ~ESSnusbPhysicsList();

    virtual void ConstructParticle() override;
    virtual void ConstructProcess() override;

    virtual void SetCuts() override;
    // Sets a cut value for all particle types in the particle table.
private:
    // Physics
    std::vector<G4VPhysicsConstructor*> fProcesses;
    std::vector<G4VPhysicsConstructor*> fcustomProcesses;

    ClassDef(ESSnusbPhysicsList, 2);

    void AddCustomProcesses();
};

} // namespace physicsList
} // namespace core
} // namespace esbroot
#endif
