#ifndef ESBROOT_GENERATOR_ND_SIMPLE_GENERATOR_H
#define ESBROOT_GENERATOR_ND_SIMPLE_GENERATOR_H 1

#include "core/generator/IGenerator.hpp"

#include <cmath>

#include "TObject.h"

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4PhysicalConstants.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4ThreeVector.hh"

#include "G4SystemOfUnits.hh"
#include "G4Event.hh"

#include "G4GeneralParticleSource.hh"

namespace esbroot {
namespace generators {
namespace ndcherenkov {


class NDSimpleGenerator : public core::generator::IGenerator , public TObject
{
public:

	//! Default constructor
	NDSimpleGenerator();
	
	//! Destructor
	~NDSimpleGenerator();
	
    void IGeneratePrimaries(G4Event* anEvent) override;

private:
	//G4ParticleGun *fParticleGun{nullptr}; //!<!
	//G4GeneralParticleSource* gps{nullptr}; //!<!

	ClassDef(NDSimpleGenerator,3)
};

} //namespace ndcherenkov 
} //namespace generators
} //namespace esbroot

#endif
