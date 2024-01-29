#include "ESSnusbPhysicsList.hpp"

#include "G4BaryonConstructor.hh"
#include "G4BosonConstructor.hh"
#include "G4IonConstructor.hh"
#include "G4LeptonConstructor.hh"
#include "G4MesonConstructor.hh"
#include "G4ShortLivedConstructor.hh"

namespace esbroot {
namespace esbcore {
namespace physicslist {

ESSnusbPhysicsList::ESSnusbPhysicsList() {}

ESSnusbPhysicsList::~ESSnusbPhysicsList() {}

void ESSnusbPhysicsList::ConstructParticle() {
  // Construct all bosons
  G4BosonConstructor bosons;
  bosons.ConstructParticle();

  // Construct all leptons
  G4LeptonConstructor leptons;
  leptons.ConstructParticle();

  // Construct all mesons
  G4MesonConstructor mesons;
  mesons.ConstructParticle();

  // Construct all baryons
  G4BaryonConstructor baryons;
  baryons.ConstructParticle();

  // Construct all ions
  G4IonConstructor ions;
  ions.ConstructParticle();

  // Construct all short lived particles
  G4ShortLivedConstructor shortLived;
  shortLived.ConstructParticle();
}

void ESSnusbPhysicsList::ConstructProcess() {}

void ESSnusbPhysicsList::SetCuts() {
  G4VUserPhysicsList::SetCuts(); // TODO set essnusbCuts
}

} // namespace physicslist
} // namespace esbcore
} // namespace esbroot
