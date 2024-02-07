#include "EsbActionInitializer.hpp"
ClassImp(esbroot::core::simulation::EsbActionInitializer)

#include "EsbRunAction.hpp"
#include "EsbEventAction.hpp"

#include "G4GeneralParticleSource.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"


namespace esbroot {
namespace core {
namespace simulation {

EsbActionInitializer::EsbActionInitializer() :
  G4VUserActionInitialization()
{;}

EsbActionInitializer::~EsbActionInitializer()
{;}


void EsbActionInitializer::Build() const
{
  // primary generator
  SetUserAction(fGenerator);

  //Thread-local RunAction: same class, but code controlled by IsMaster()
  SetUserAction(new EsbRunAction());
  SetUserAction(new EsbEventAction());
}

void EsbActionInitializer::BuildForMaster() const
{
  //Thread-local RunAction: same class, but code controlled by IsMaster()
  SetUserAction(new EsbRunAction());
}

} // namespace simulation
} // namespace core
} // namespace esbroot
