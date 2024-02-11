#pragma once

#ifndef EsbIGenerator_hpp
#define EsbIGenerator_hpp 1

#include "TObject.h"

#include "G4Event.hh"
#include "G4ParticleTable.hh"

namespace esbroot {
namespace core {
namespace generator {

class IGenerator : public TObject
{
    public:
        IGenerator();
        virtual ~IGenerator() = default;

        virtual void IGeneratePrimaries(G4Event* anEvent);

private:
    ClassDef(IGenerator, 2);
};

} // namespace generator
} // namespace core
} // namespace esbroot

#endif
