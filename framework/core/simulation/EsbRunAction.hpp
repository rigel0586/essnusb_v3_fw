#ifndef EsbRunAction_h
#define EsbRunAction_h 1

#include "TObject.h"

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

  EsbRunAction();
  ~EsbRunAction();
  
public:
  
  void BeginOfRunAction(const G4Run* aRun);
  void EndOfRunAction(const G4Run* aRun);  

private:  

  ClassDef(EsbRunAction, 2);
};

} // namespace simulation
} // namespace core
} // namespace esbroot

#endif 
