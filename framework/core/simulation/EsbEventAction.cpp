#include "EsbEventAction.hpp"
ClassImp(esbroot::core::simulation::EsbEventAction)

#include "G4Event.hh"

namespace esbroot {
namespace core {
namespace simulation {

EsbEventAction::EsbEventAction(const std::vector<detector::IDetector*>& detectors)
  : fDetectors(detectors)
{;}


EsbEventAction::~EsbEventAction(){;}

void EsbEventAction::BeginOfEventAction(const G4Event* evt)
{
    for(detector::IDetector* d : fDetectors)
    {
        d->BeginOfEventAction(evt);
    }
}

void EsbEventAction::EndOfEventAction(const G4Event* evt)
{
    for(detector::IDetector* d : fDetectors)
    {
        d->EndOfEventAction(evt);
    }
}

} // namespace simulation
} // namespace core
} // namespace esbroot
