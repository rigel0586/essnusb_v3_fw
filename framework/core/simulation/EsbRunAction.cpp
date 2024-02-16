#include "EsbRunAction.hpp"
ClassImp(esbroot::core::simulation::EsbRunAction)

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"
#include "Randomize.hh"
#include <ctime>
#include "Randomize.hh"

#include "EsbWriterPersistency.hpp"

namespace esbroot {
namespace core {
namespace simulation {

EsbRunAction::EsbRunAction(const std::vector<detector::IDetector*>& detectors)
  : fDetectors(detectors)
{
}

EsbRunAction::~EsbRunAction()
{;}


void EsbRunAction::BeginOfRunAction(const G4Run* aRun)
{
  for(detector::IDetector* d : fDetectors)
  {
      d->BeginOfRunAction(aRun);
  }
  return;
}

void EsbRunAction::EndOfRunAction(const G4Run* aRun)
{
  for(detector::IDetector* d : fDetectors)
  {
      d->EndOfRunAction(aRun);
  }
  io::EsbWriterPersistency::Instance().writeData();
  return;
}

} // namespace simulation
} // namespace core
} // namespace esbroot