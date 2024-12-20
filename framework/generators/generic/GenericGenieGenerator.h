#ifndef ESBROOT_GENERATOR_GENERIC_GENIE_GENERATOR_H
#define ESBROOT_GENERATOR_GENERIC_GENIE_GENERATOR_H 1

#include "GenieGenerator.h"
#include "generators/generic/IFluxNextPosition.h"
#include "core/generator/IGenerator.hpp"
#include "utility/Utility.hpp"

#include <cmath>

#include "TGeoManager.h"
#include "TGeoVolume.h"
#include "TObject.h"

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4PhysicalConstants.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4ThreeVector.hh"

#include "G4SystemOfUnits.hh"
#include "G4Event.hh"

class TVector3;
class TLorentzVector;

namespace esbroot {
namespace generators {
namespace generic {


class GenericGenieGenerator : public esbroot::generators::generic::GenieGenerator
						, public core::generator::IGenerator
						, public TObject
{
public:

	//! Default constructor
	GenericGenieGenerator();
	
	// ! Basic constructor
	GenericGenieGenerator(IFluxNextPosition* fluxPosition
									, const std::string& volumeName
									, const char* nuFluxFile
									, unsigned int seed
									, Int_t numEvents
									, genie::GFluxI* extFlux = nullptr
									, Bool_t uniformFlux = false
									, TGeoManager* gm = nullptr
									, Bool_t keepThrowingFluxNu = true);

	// ! Composite constructor
	GenericGenieGenerator(std::vector<IFluxNextPosition*> fluxPositions
									, const std::string& volumeName
									, const char* nuFluxFile
									, unsigned int seed
									, Int_t numEvents
									, genie::GFluxI* extFlux = nullptr
									, Bool_t uniformFlux = false
									, TGeoManager* gm = nullptr
									, Bool_t keepThrowingFluxNu = true);

	//! Destructor
	~GenericGenieGenerator();
	
	//! Implementation of GenieGenerator::Init() which calls the Configure() function.
	virtual Bool_t Init() {return(Configure());};
	
	//! Post processes Genie events to choose vertex position
	virtual void PostProcessEvent(/*IN OUT*/ genie::GHepRecord* event) override;

	virtual Bool_t Configure() override; 

	void UseFixVertex(Bool_t fv) {fUseFixedVertex = fv;}
	void SetVertexPos(const TVector3& vp ) {fvertexPos = vp;}

    void IGeneratePrimaries(G4Event* anEvent) override;
    G4ParticleGun* GetParticleGun(){return fparticleGun;} 

protected:
	virtual Bool_t KeepThrowing(std::vector<genie::GHepParticle*>& eventParticles ) override;
	virtual void WriteToOutputFile(const genie::EventRecord* event, Bool_t flaGkeepThrowing ) override;

private:
	enum class GeneratorType{
      Basic,
      Composite
    };

	TGeoManager* fgm;//!<!
	std::string fgeoConfigFile;//!<!
	std::string fnuFluxFile;//!<!
	unsigned int fseed;//!<!
	Int_t fnumEvents;//!<!
	Int_t fCurrentEvent;//!<!
	Bool_t fUseFixedVertex;//!<!
	TVector3 fvertexPos;//!<!
	genie::GFluxI* fExtFlux;//!<!
	Bool_t fUseUniformflux;//!<!
	Bool_t fKeepThrowingFluxNu;//!<!

	IFluxNextPosition* fFluxPosition;//!<!
	CompositeIFluxNextPosition* fCompositeFlux{nullptr};
	std::vector<IFluxNextPosition*> fCompositeFluxPositions;//!<!
	GeneratorType fGenType;

	const std::string fvolumeName;//!<!

	std::vector<genie::EventRecord> fGenieEvents;//!<!
	void GenerateEvents();

    G4ParticleGun* fparticleGun{nullptr};
    G4ParticleTable * fg4ParticleTable{nullptr};
	
	utility::Utility f_util;
	
	ClassDef(GenericGenieGenerator,3)
};

} //namespace generic 
} //namespace generators
} //namespace esbroot

#endif
