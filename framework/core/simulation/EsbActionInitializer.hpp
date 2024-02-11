#ifndef EsbActionInitializer_h
#define EsbActionInitializer_h 1

#include "core/generator/IGenerator.hpp"

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

  EsbActionInitializer(generator::IGenerator* generator);
  ~EsbActionInitializer();
  
  void Build() const override;
  void BuildForMaster() const override;

private:

    class G4Generator : public G4VUserPrimaryGeneratorAction
    {
      public:
          G4Generator(generator::IGenerator* generator) : fgenerator(generator) {}
          ~G4Generator() = default;

          void 	GeneratePrimaries (G4Event *anEvent) override {
                    if(fgenerator != nullptr) fgenerator->IGeneratePrimaries(anEvent);
          }
      private:
          generator::IGenerator* fgenerator{nullptr};
    };

    G4Generator* fGenerator{nullptr};

    ClassDef(EsbActionInitializer, 2);
};

} // namespace simulation
} // namespace core
} // namespace esbroot


#endif

