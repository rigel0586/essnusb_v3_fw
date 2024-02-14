#ifndef EsbRunAction_h
#define EsbRunAction_h 1

#include <vector>

#include "TObject.h"

#include "core/detector/IDetector.hpp"

#include "globals.hh"
#include "G4UserRunAction.hh"

// forward declarations
class G4Run;

namespace esbroot {
namespace core {
namespace simulation {

class EsbRunAction : public TObject, 
                      public G4UserRunAction
{
  public:

  EsbRunAction(const std::vector<detector::IDetector*>& detectors);
  ~EsbRunAction();
  
public:
  
  void BeginOfRunAction(const G4Run* aRun);
  void EndOfRunAction(const G4Run* aRun);  

private:  
  const std::vector<detector::IDetector*> fDetectors;

  ClassDef(EsbRunAction, 2);
};

} // namespace simulation
} // namespace core
} // namespace esbroot

#endif 
