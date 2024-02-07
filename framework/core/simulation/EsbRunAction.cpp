#include "EsbRunAction.hpp"
ClassImp(esbroot::core::simulation::EsbRunAction)

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"
#include "Randomize.hh"
#include <ctime>
#include "Randomize.hh"


namespace esbroot {
namespace core {
namespace simulation {

EsbRunAction::EsbRunAction()
{
}

EsbRunAction::~EsbRunAction()
{;}


void EsbRunAction::BeginOfRunAction(const G4Run* aRun)
{
  // TODO decide if we need something
  return;
}

void EsbRunAction::EndOfRunAction(const G4Run* aRun)
{
  // TODO add export geometry etc. for archiving
  return;
}

} // namespace simulation
} // namespace core
} // namespace esbroot