#include "ESSnusbPhysicsList.hpp"
ClassImp(esbroot::core::physicsList::ESSnusbPhysicsList)

#include "G4BaryonConstructor.hh"
#include "G4BosonConstructor.hh"
#include "G4IonConstructor.hh"
#include "G4LeptonConstructor.hh"
#include "G4MesonConstructor.hh"
#include "G4ShortLivedConstructor.hh"

#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleWithCuts.hh"
#include "G4ParticleTable.hh"
#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4ProcessManager.hh"
#include "G4ProcessVector.hh"

#include "G4DecayPhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4EmLivermorePhysics.hh"
#include "G4EmPenelopePhysics.hh"
#include "G4EmLowEPPhysics.hh"
#include "G4EmExtraPhysics.hh"
#include "G4HadronElasticPhysics.hh"
#include "G4IonPhysics.hh"
#include "G4OpticalPhysics.hh"
#include "G4StoppingPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4VHadronPhysics.hh"

#include "G4NeutronElasticXS.hh"
#include "G4NeutronInelasticXS.hh"
#include "G4NeutronCaptureXS.hh"

#include "G4HadronElasticPhysicsXS.hh"
#include "G4IonBinaryCascadePhysics.hh"
#include "G4HadronInelasticQBBC.hh"
#include "G4ThermalNeutrons.hh"

namespace esbroot {
namespace core {
namespace physicsList {

ESSnusbPhysicsList::ESSnusbPhysicsList(std::vector<G4VPhysicsConstructor*>& customProcesses) 
    : G4VModularPhysicsList(), fcustomProcesses(customProcesses)
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

    // Optical
    G4OpticalPhysics* optP = new G4OpticalPhysics(ver);
    fProcesses.emplace_back(static_cast<G4VPhysicsConstructor*>(optP));
    RegisterPhysics( optP );

    // Other
    G4StoppingPhysics* stopP = new G4StoppingPhysics(ver);
    fProcesses.emplace_back(static_cast<G4VPhysicsConstructor*>(stopP));
    RegisterPhysics( stopP );

    // Radioactive decay
    G4RadioactiveDecayPhysics* rActiveP = new G4RadioactiveDecayPhysics();
    fProcesses.emplace_back(static_cast<G4VPhysicsConstructor*>(rActiveP));
    RegisterPhysics (rActiveP);

    // Hadron physics
    G4HadronElasticPhysics* hadElP = new G4HadronElasticPhysics(ver);
    fProcesses.emplace_back(static_cast<G4VPhysicsConstructor*>(hadElP));
    RegisterPhysics (hadElP);

    G4HadronInelasticQBBC* hadInElPhQbbc = new G4HadronInelasticQBBC(ver);
    fProcesses.emplace_back(static_cast<G4VPhysicsConstructor*>(hadInElPhQbbc));
    RegisterPhysics (hadInElPhQbbc);

    G4HadronElasticPhysicsXS* hadElPhXs = new G4HadronElasticPhysicsXS(ver);
    fProcesses.emplace_back(static_cast<G4VPhysicsConstructor*>(hadElPhXs));
    RegisterPhysics (hadElPhXs);

    G4IonBinaryCascadePhysics* hadIonBCPh = new G4IonBinaryCascadePhysics(ver);
    fProcesses.emplace_back(static_cast<G4VPhysicsConstructor*>(hadIonBCPh));
    RegisterPhysics (hadIonBCPh);


    G4ThermalNeutrons* hadTherNeu = new G4ThermalNeutrons(ver);
    fProcesses.emplace_back(static_cast<G4VPhysicsConstructor*>(hadTherNeu));
    RegisterPhysics (hadTherNeu);

    AddCustomProcesses();
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

void ESSnusbPhysicsList::AddCustomProcesses()
{
    for(G4VPhysicsConstructor* pr : fcustomProcesses)
    {
        fProcesses.emplace_back(pr);
        RegisterPhysics(pr); 
    }
}

} // namespace physicsList
} // namespace core
} // namespace esbroot
