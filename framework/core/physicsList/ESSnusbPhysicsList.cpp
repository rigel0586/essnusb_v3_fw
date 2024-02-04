#include "ESSnusbPhysicsList.hpp"
ClassImp(esbroot::core::physicsList::ESSnusbPhysicsList)

#include "G4BaryonConstructor.hh"
#include "G4BosonConstructor.hh"
#include "G4IonConstructor.hh"
#include "G4LeptonConstructor.hh"
#include "G4MesonConstructor.hh"
#include "G4ShortLivedConstructor.hh"

namespace esbroot {
namespace core {
namespace physicsList {

ESSnusbPhysicsList::ESSnusbPhysicsList() : G4VModularPhysicsList()
{
    G4int ver = 1;
    SetVerboseLevel(ver);

    // EM Physics
    RegisterPhysics( new G4EmStandardPhysics(ver) );
    RegisterPhysics( new G4EmLivermorePhysics(ver) );
    RegisterPhysics( new G4EmPenelopePhysics(ver) );
    RegisterPhysics( new G4EmLowEPPhysics(ver) );

    // Synchroton Radiation & GN Physics
    RegisterPhysics( new G4EmExtraPhysics(ver) );

    // Decays
    RegisterPhysics( new G4DecayPhysics(ver) );

    // Ions
    RegisterPhysics( new G4IonPhysics(ver) );

    // Hadron physics
    RegisterPhysics( new G4HadronElasticPhysics(ver) );
    
    // Optical
    RegisterPhysics( new G4OpticalPhysics(ver));

    // Other
    RegisterPhysics( new G4StoppingPhysics(ver) );
}

ESSnusbPhysicsList::~ESSnusbPhysicsList() {}

void ESSnusbPhysicsList::ConstructParticle() {
  
}

void ESSnusbPhysicsList::ConstructProcess(){
    AddTransportation();
}

void ESSnusbPhysicsList::SetCuts() {
  if (verboseLevel >1){
    G4cout << "ESSnusbPhysicsList::SetCuts:";
  }
  //  " G4VUserPhysicsList::SetCutsWithDefault" method sets
  //   the default cut value for all particle types
  SetCutsWithDefault();
}

} // namespace physicsList
} // namespace core
} // namespace esbroot
