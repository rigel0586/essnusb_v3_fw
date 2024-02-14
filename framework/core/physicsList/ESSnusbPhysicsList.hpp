#pragma once

#ifndef ESSnusbPhysicsList_hpp
#define ESSnusbPhysicsList_hpp 1

#include "G4ios.hh"
//#include "iomanip.h"
#include "globals.hh"

#include "G4VModularPhysicsList.hh"

#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleWithCuts.hh"
#include "G4ParticleTable.hh"
#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4ProcessManager.hh"
#include "G4ProcessVector.hh"

#include "G4EmStandardPhysics.hh"
#include "G4EmLivermorePhysics.hh"
#include "G4EmPenelopePhysics.hh"
#include "G4EmLowEPPhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4IonPhysics.hh"
#include "G4EmExtraPhysics.hh"
#include "G4StoppingPhysics.hh"
#include "G4OpticalPhysics.hh"
#include "G4HadronElasticPhysics.hh"

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
    ESSnusbPhysicsList();
    virtual ~ESSnusbPhysicsList();

    virtual void ConstructParticle() override;
    virtual void ConstructProcess() override;

    virtual void SetCuts() override;
    // Sets a cut value for all particle types in the particle table.
private:
    // Physics
    std::vector<G4VPhysicsConstructor*> fProcesses;

    ClassDef(ESSnusbPhysicsList, 2);
};

} // namespace physicsList
} // namespace core
} // namespace esbroot
#endif
