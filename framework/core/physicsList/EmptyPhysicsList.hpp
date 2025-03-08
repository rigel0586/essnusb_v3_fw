#pragma once

#ifndef ESSnusbPEmptyhysicsList_hpp
#define ESSnusbPEmptyhysicsList_hpp 1

#include "G4VUserPhysicsList.hh"

#include "TObject.h"

namespace esbroot {
namespace core {
namespace physicsList {


class EmptyPhysicsList : public TObject,public G4VUserPhysicsList
{
  protected:
    void ConstructParticle();
    void ConstructProcess();

private:
    ClassDef(EmptyPhysicsList, 2);
};

} // namespace physicsList
} // namespace core
} // namespace esbroot
#endif
