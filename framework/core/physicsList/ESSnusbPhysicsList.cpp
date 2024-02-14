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
    G4EmStandardPhysics* emStad = new G4EmStandardPhysics(ver);
    fProcesses.emplace_back(static_cast<G4VPhysicsConstructor*>(emStad));
    RegisterPhysics( emStad );

    // G4EmLivermorePhysics* emLp = new G4EmLivermorePhysics(ver);
    // fProcesses.emplace_back(static_cast<G4VPhysicsConstructor*>(emLp));
    // RegisterPhysics( emLp );

    // G4EmPenelopePhysics* emPP = new G4EmPenelopePhysics(ver);
    // fProcesses.emplace_back(static_cast<G4VPhysicsConstructor*>(emPP));
    // RegisterPhysics( emPP );

    // G4EmLowEPPhysics* emLEP = new G4EmLowEPPhysics(ver);
    // fProcesses.emplace_back(static_cast<G4VPhysicsConstructor*>(emLEP));
    // RegisterPhysics( emLEP );

    // Synchroton Radiation & GN Physics
    G4EmExtraPhysics* emEP = new G4EmExtraPhysics(ver);
    fProcesses.emplace_back(static_cast<G4VPhysicsConstructor*>(emEP));
    RegisterPhysics( emEP );

    // Decays
    G4DecayPhysics* decP = new G4DecayPhysics(ver);
    fProcesses.emplace_back(static_cast<G4VPhysicsConstructor*>(decP));
    RegisterPhysics( decP );

    // Ions
    G4IonPhysics* ionP = new G4IonPhysics(ver);
    fProcesses.emplace_back(static_cast<G4VPhysicsConstructor*>(ionP));
    RegisterPhysics( ionP );

    // Hadron physics
//    RegisterPhysics( new G4HadronElasticPhysics(ver) );
    
    // Optical
    G4OpticalPhysics* optP = new G4OpticalPhysics(ver);
    fProcesses.emplace_back(static_cast<G4VPhysicsConstructor*>(optP));
    RegisterPhysics( optP );

    // Other
    G4StoppingPhysics* stopP = new G4StoppingPhysics(ver);
    fProcesses.emplace_back(static_cast<G4VPhysicsConstructor*>(stopP));
    RegisterPhysics( stopP );
}

ESSnusbPhysicsList::~ESSnusbPhysicsList() {}

void ESSnusbPhysicsList::ConstructParticle() {
    // Construct all bosons
    G4BosonConstructor      bosons;
    bosons.ConstructParticle();

    // Construct all leptons
    G4LeptonConstructor     leptons;
    leptons.ConstructParticle();
      
    // Construct all mesons
    G4MesonConstructor      mesons;
    mesons.ConstructParticle();
    
    // Construct all baryons
    G4BaryonConstructor     baryons;
    baryons.ConstructParticle();
    
    // Construct all ions
    G4IonConstructor        ions;
    ions.ConstructParticle();

    // Construct all short lived particles
    G4ShortLivedConstructor shortLived;
    shortLived.ConstructParticle();

    for(G4VPhysicsConstructor* pr : fProcesses)
    {
        pr->ConstructParticle();
    }
}

void ESSnusbPhysicsList::ConstructProcess(){
    AddTransportation();

    for(G4VPhysicsConstructor* pr : fProcesses)
    {
        pr->ConstructProcess();
    }
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
