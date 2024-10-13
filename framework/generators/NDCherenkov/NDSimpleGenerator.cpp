#include "NDCherenkov/NDSimpleGenerator.h"
ClassImp(esbroot::generators::ndcherenkov::NDSimpleGenerator)


#include <Framework/Conventions/Units.h>
#include "Framework/GHEP/GHepParticle.h"
#include "Framework/Interaction/ProcessInfo.h"
#include "Framework/Interaction/Interaction.h"
#include "Framework/Interaction/InitialState.h"

#include <fairlogger/Logger.h>

#include <G4ParticleTable.hh>

namespace esbroot {
namespace generators {
namespace ndcherenkov {

NDSimpleGenerator::NDSimpleGenerator()
{
}

NDSimpleGenerator::~NDSimpleGenerator()
{
	delete fParticleGun;
	delete gps;
}

void NDSimpleGenerator::IGeneratePrimaries(G4Event* anEvent)
{
	if(fParticleGun == nullptr){
		fParticleGun = new G4ParticleGun(1);
	}
		
	if(gps == nullptr){
		gps = new G4GeneralParticleSource();
	}

	G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
	G4String particleName = "neutron";
	G4ParticleDefinition *particle = particleTable->FindParticle("neutron");
	
	G4ThreeVector pos(0.,0.,0.);
	G4ThreeVector mom(0.,0.,1.);
	
	fParticleGun->SetParticlePosition(pos);
	fParticleGun->SetParticleMomentumDirection(mom);
	fParticleGun->SetParticleEnergy(5. *MeV);
	fParticleGun->SetParticleDefinition(particle);
	
	fParticleGun->GeneratePrimaryVertex(anEvent);
	
	gps->GetCurrentSource()->GetAngDist()->SetParticleMomentumDirection(mom);
    gps->GetCurrentSource()->GetPosDist()->SetCentreCoords(pos);
	gps->GeneratePrimaryVertex(anEvent);
}

} //namespace ndcherenkov 
} //namespace generators
} //namespace esbroot
