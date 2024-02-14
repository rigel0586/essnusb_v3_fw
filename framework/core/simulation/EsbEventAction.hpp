#ifndef EsbEventAction_h
#define EsbEventAction_h 1

#include <vector>

#include "TObject.h"

#include "core/detector/IDetector.hpp"

#include "G4UserEventAction.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"

namespace esbroot {
namespace core {
namespace simulation {
  
class EsbEventAction : public TObject, 
                        public G4UserEventAction
{
  public:
    EsbEventAction(const std::vector<detector::IDetector*>& detectors);
   ~EsbEventAction();

  public:
    void BeginOfEventAction(const G4Event*);
    void EndOfEventAction(const G4Event*);

  private:
    const std::vector<detector::IDetector*> fDetectors;
 
    ClassDef(EsbEventAction, 2);
};

} // namespace simulation
} // namespace core
} // namespace esbroot
    
#endif
