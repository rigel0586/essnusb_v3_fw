#pragma once

#ifndef ESSnusbPhysicsList_hpp
#define ESSnusbPhysicsList_hpp 1

#include "G4VUserPhysicsList.hh"

namespace esbcore {
namespace physicslist {

class ESSnusbPhysicsList : public G4VUserPhysicsList {
public:
  ESSnusbPhysicsList();
  virtual ~ESSnusbPhysicsList();

  virtual void ConstructParticle() override;
  virtual void ConstructProcess() override;

  virtual void SetCuts() override;
  // Sets a cut value for all particle types in the particle table.
};

} // namespace physicslist
} // namespace esbcore

#endif
