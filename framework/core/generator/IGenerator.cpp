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

} // namespace generator
} // namespace core
} // namespace esbroot
