#include "EsbActionInitializer.hpp"
ClassImp(esbroot::core::simulation::EsbActionInitializer)

#include "EsbRunAction.hpp"
#include "EsbEventAction.hpp"

#include "G4GeneralParticleSource.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"

#include "generators/SuperFGD/FgdGenieGenerator.h"


namespace esbroot {
namespace core {
namespace simulation {

EsbActionInitializer::EsbActionInitializer(generator::IGenerator* generator, std::vector<detector::IDetector*>& detectors) 
  : G4VUserActionInitialization(), fGenerator(new G4Generator(generator)), fDetectors(detectors)
{;}

EsbActionInitializer::~EsbActionInitializer()
{;}


void EsbActionInitializer::Build() const
{
  std::cout << "EsbActionInitializer::Build " << std::endl;
  // primary generator
  SetUserAction(fGenerator);

  //Thread-local RunAction: same class, but code controlled by IsMaster()
  SetUserAction(new EsbRunAction(fDetectors));
  SetUserAction(new EsbEventAction(fDetectors));
}

void EsbActionInitializer::BuildForMaster() const
{
  //Thread-local RunAction: same class, but code controlled by IsMaster()
  SetUserAction(new EsbRunAction(fDetectors));
}

} // namespace simulation
} // namespace core
} // namespace esbroot
