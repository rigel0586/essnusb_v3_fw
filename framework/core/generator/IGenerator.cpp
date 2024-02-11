#include "IGenerator.hpp"
ClassImp(esbroot::core::generator::IGenerator)

#include "TObject.h"
#include "G4Event.hh"

namespace esbroot {
namespace core {
namespace generator {

IGenerator::IGenerator(){}


void IGenerator::IGeneratePrimaries(G4Event* anEvent)
{
}

void IGenerator::setG4ParticleTable(G4ParticleTable* pt)
{
}

} // namespace generator
} // namespace core
} // namespace esbroot
