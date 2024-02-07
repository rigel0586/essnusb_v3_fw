#include "EsbEventAction.hpp"
ClassImp(esbroot::core::simulation::EsbEventAction)

#include "G4Event.hh"

namespace esbroot {
namespace core {
namespace simulation {

EsbEventAction::EsbEventAction()
{;}


EsbEventAction::~EsbEventAction(){;}

void EsbEventAction::BeginOfEventAction(const G4Event* evt)
{
  // TODO decide what we need
}

void EsbEventAction::EndOfEventAction(const G4Event* evt)
{
  // TODO decide what we need
}

} // namespace simulation
} // namespace core
} // namespace esbroot
