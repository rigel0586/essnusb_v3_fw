#pragma once

#ifndef EsbEmptyPrimaryGenerator_hpp
#define EsbEmptyPrimaryGenerator_hpp 1

#include "TObject.h"
#include "G4VUserPrimaryGeneratorAction.hh"

namespace esbroot {
namespace core {
namespace generator {

class EmptyPrimaryGenerator : public TObject , public G4VUserPrimaryGeneratorAction
{
public:
    EmptyPrimaryGenerator();
    virtual void GeneratePrimaries(G4Event*) { };

private:
    ClassDef(EmptyPrimaryGenerator, 2);
};

} // namespace generator
} // namespace core
} // namespace esbroot

#endif
