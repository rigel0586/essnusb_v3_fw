#ifndef EsbActionInitializer_h
#define EsbActionInitializer_h 1

#include "TObject.h"

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4VUserActionInitialization.hh"

namespace esbroot {
namespace core {
namespace simulation {

class EsbActionInitializer : public TObject, 
                                public G4VUserActionInitialization
{
public:

  EsbActionInitializer();
  ~EsbActionInitializer();
  
  void Build() const;
  void BuildForMaster() const;

  void setGenerator(G4VUserPrimaryGeneratorAction* f){fGenerator = f;}

private:
  G4VUserPrimaryGeneratorAction* fGenerator{nullptr};
};

} // namespace simulation
} // namespace core
} // namespace esbroot


#endif

