#include "generic/GenericGenieGenerator.h"
ClassImp(esbroot::generators::generic::GenericGenieGenerator)

#include "generic/GenieFluxDriver.h"
#include "generic/GenericGeomAnalyzer.h"

#include <Framework/Conventions/Units.h>
#include "Framework/GHEP/GHepParticle.h"
#include "Framework/Interaction/ProcessInfo.h"
#include "Framework/Interaction/Interaction.h"
#include "Framework/Interaction/InitialState.h"

#include <Tools/Geometry/ROOTGeomAnalyzer.h>

#include <fairlogger/Logger.h>

#include <G4ParticleTable.hh>

namespace esbroot {
namespace generators {
namespace generic {

GenericGenieGenerator::GenericGenieGenerator()
	: GenieGenerator()
{
}

GenericGenieGenerator::~GenericGenieGenerator()
{
	delete fCompositeFlux;
    delete fparticleGun;
}

GenericGenieGenerator::GenericGenieGenerator(IFluxNextPosition* fluxPosition
									, const std::string& volumeName
									, const char* nuFluxFile
									, unsigned int seed
									, Int_t numEvents
									, genie::GFluxI* extFlux
									, Bool_t uniformFlux
									, TGeoManager* gm
									, Bool_t keepThrowingFluxNu)
	 : esbroot::generators::generic::GenieGenerator()
	 	, fFluxPosition(fluxPosition)
		, fGenType(GeneratorType::Basic)
		, fvolumeName(volumeName)
		, fnuFluxFile(nuFluxFile)
		, fseed(seed)
		, fnumEvents(numEvents)
		, fgm(gm)
		, fExtFlux(extFlux)
		, fCurrentEvent(0)
		, fUseFixedVertex(false)
		, fvertexPos(0,0,0)
		, fUseUniformflux(uniformFlux)
		, fKeepThrowingFluxNu(keepThrowingFluxNu)
{
}

GenericGenieGenerator::GenericGenieGenerator(std::vector<IFluxNextPosition*> fluxPositions
									, const std::string& volumeName
									, const char* nuFluxFile
									, unsigned int seed
									, Int_t numEvents
									, genie::GFluxI* extFlux
									, Bool_t uniformFlux
									, TGeoManager* gm
									, Bool_t keepThrowingFluxNu)
	 : esbroot::generators::generic::GenieGenerator()
	 	, fCompositeFluxPositions(fluxPositions)
		, fGenType(GeneratorType::Composite)
		, fvolumeName(volumeName)
		, fnuFluxFile(nuFluxFile)
		, fseed(seed)
		, fnumEvents(numEvents)
		, fgm(gm)
		, fExtFlux(extFlux)
		, fCurrentEvent(0)
		, fUseFixedVertex(false)
		, fvertexPos(0,0,0)
		, fUseUniformflux(uniformFlux)
		, fKeepThrowingFluxNu(keepThrowingFluxNu)
{
}


void GenericGenieGenerator::PostProcessEvent(/*IN OUT*/ genie::GHepRecord* event)
{
	if(fUseFixedVertex)
	{
		event->SetVertex(fvertexPos.X() * f_util.rootToG4CoeffLength()
							, fvertexPos.Y() * f_util.rootToG4CoeffLength()
							, fvertexPos.Z() * f_util.rootToG4CoeffLength()
							,0);
	}
}


Bool_t GenericGenieGenerator::Configure()
{
	if(fgm==nullptr)
	{
		fgm = gGeoManager;
	}

	GenericGeomAnalyzer* geomAnalyzer = nullptr;
	bool useDefaultFlux = (fExtFlux==nullptr);


	if(useDefaultFlux)
	{
		LOG(info) 	<< "Using flux driver - GenieFluxDriver [Generic implementation], with top volume : " 
					<< fvolumeName;

		auto fgdGeom = std::make_shared<GenericGeomAnalyzer>(fgm, fvolumeName);
		fgdGeom->SetScannerNParticles(fnumEvents);
		SetGeomI(fgdGeom);

		if(fGenType == GeneratorType::Basic){
			auto gFluxD = std::make_shared<GenieFluxDriver>(fnuFluxFile.c_str(), fFluxPosition, fseed, fUseUniformflux);
			gFluxD->SetMaxEvents(fnumEvents);
			SetFluxI(gFluxD);
		} else if(fGenType == GeneratorType::Composite){
			fCompositeFlux = new CompositeIFluxNextPosition(fCompositeFluxPositions);
			auto gFluxD = std::make_shared<GenieFluxDriver>(fnuFluxFile.c_str(), static_cast<IFluxNextPosition*>(fCompositeFlux), fseed, fUseUniformflux);
			gFluxD->SetMaxEvents(fnumEvents);
			SetFluxI(gFluxD);
		} else{
			LOG(fatal) <<  "Unknown type of GeneratorType ...";
        	exit(0);
		}
		

		geomAnalyzer = dynamic_cast<GenericGeomAnalyzer*>(GetGeomI().get());
		geomAnalyzer->SetScannerFlux(GetFluxI().get()); // Force to use MaxPathLengthsFluxMethod, otherwise it uses MaxPathLengthsBoxMethod
	}
	else
	{
		LOG(info) << "Using external flux driver with top volume : " << (fgm->GetTopVolume())->GetName();

		auto rootGeom = std::make_shared<genie::geometry::ROOTGeomAnalyzer>(fgm);
		rootGeom->SetScannerNParticles(fnumEvents);

		SetGeomI(rootGeom);
		SetFluxI(shared_ptr<genie::GFluxI>(fExtFlux));	
	}

	esbroot::generators::generic::GenieGenerator::Configure();
	fmcj_driver->KeepOnThrowingFluxNeutrinos(fKeepThrowingFluxNu);
	// GenerateEvents();
	
	// if(useDefaultFlux && geomAnalyzer!=nullptr)
	// {
	// 	geomAnalyzer->Reset();	// Revert initial Top Volume Geometry
	// }

	return true;
}

void GenericGenieGenerator::IGeneratePrimaries(G4Event* anEvent)
{
	if(fg4ParticleTable == nullptr)
		fg4ParticleTable = G4ParticleTable::GetParticleTable();

	if(fparticleGun == nullptr)
		fparticleGun = new G4ParticleGun();

    if(fparticleGun==nullptr || fg4ParticleTable == nullptr) return;

	if(!IsConfigured()){
		Configure();
	}

    while(true)
	{
		genie::EventRecord* event = fmcj_driver->GenerateEvent();
		if(event != nullptr)
		{
			PostProcessEvent(event);
            event->Print(std::cout);
            TLorentzVector* v = event->Vertex();
			int nParticles = event->GetEntries();
			std::vector<genie::GHepParticle*> eventParticles;
			for (int i = 0; i < nParticles; i++) 
			{
				genie::GHepParticle *p = event->Particle(i);
				// kIStStableFinalState - Genie documentation: generator-level final state
				// particles to be tracked by the detector-level MC
				if ((p->Status() == genie::EGHepStatus::kIStStableFinalState)) 
				{
					if(IsPdgAllowed(p->Pdg()))
					{
						eventParticles.push_back(p);
					}
				}
			}
			if(!KeepThrowing(eventParticles) && !eventParticles.empty())
			{
                for(genie::GHepParticle* par : eventParticles)
                {
                    G4ParticleDefinition * parDef = fg4ParticleTable->FindParticle(par->Pdg());
                    if(parDef!= nullptr)
                    {
                        // Set definition
                        fparticleGun->SetParticleDefinition(parDef);

                        // Set momentum
                        G4ParticleMomentum parMomentum;
                        parMomentum.setX(par->Px() * f_util.rootToG4CoeffEnergy());
                        parMomentum.setY(par->Py() * f_util.rootToG4CoeffEnergy());
                        parMomentum.setZ(par->Pz() * f_util.rootToG4CoeffEnergy());
						fparticleGun->SetParticleMomentum(parMomentum);

                        // Set position
                        G4ThreeVector parPosition;
                        parPosition.setX(v->X());
                        parPosition.setY(v->Y());
                        parPosition.setZ(v->Z());

                        fparticleGun->SetParticlePosition(parPosition);

                        // Set time
                        fparticleGun->SetParticleTime(par->Vt());
                        
                        // Set polarization
                        TVector3 polz;
                        par->GetPolarization(polz);

                        G4ThreeVector parPol;
                        parPol.setX(polz.x());
                        parPol.setY(polz.y());
                        parPol.setZ(polz.z());

                        fparticleGun->SetParticlePolarization(parPol);

                        // Add particle to vertex
                        fparticleGun->GeneratePrimaryVertex(anEvent);
                    }
                }

                if(!GlobalState.fOutputFileName.empty())
		        {
			        WriteToOutputFile(event, false /* flaGkeepThrowing - check made in GenerateEvents*/);
		        }

				delete event;
				break;
			}
			delete event;
        }
    }

	if(fCompositeFlux!=nullptr) fCompositeFlux->increment();
    return;
}

void GenericGenieGenerator::GenerateEvents()
{
	for(Int_t eventId = 0; eventId < fnumEvents; ++eventId)
	{
		while(true)
		{
			genie::EventRecord* event = fmcj_driver->GenerateEvent();
			if(event != nullptr)
			{
				PostProcessEvent(event);
				int nParticles = event->GetEntries();
				std::vector<genie::GHepParticle*> eventParticles;
				for (int i = 0; i < nParticles; i++) 
				{
					genie::GHepParticle *p = event->Particle(i);
					// kIStStableFinalState - Genie documentation: generator-level final state
					// particles to be tracked by the detector-level MC
					if ((p->Status() == genie::EGHepStatus::kIStStableFinalState)) 
					{
						if(IsPdgAllowed(p->Pdg()))
						{
							eventParticles.push_back(p);
						}
					}
				}
				if(!KeepThrowing(eventParticles) && !eventParticles.empty())
				{
					fGenieEvents.emplace_back(*event);
					delete event;
					break;
				}
				delete event;
			}
			else 
			{
				if(fKeepThrowingFluxNu)
				{
					--eventId; // try again
				}
				
				break;
			}
		}
	}
}

Bool_t GenericGenieGenerator::KeepThrowing(std::vector<genie::GHepParticle*>& eventParticles )
{
	static int count = 0;
	Bool_t throwAgain(false);

	if(eventParticles.empty() && count <5 /* try only 5 times */)
	{
		++count;
		throwAgain = true;
	}
	else
	{
		count = 0;
	}
	

	return throwAgain;
}

void GenericGenieGenerator::WriteToOutputFile(const genie::EventRecord* event, Bool_t flaGkeepThrowing ) 
{
	static int eventCounter = 0;
	std::ofstream outputFile(GlobalState.fOutputFileName, std::ios::app);
	if(outputFile.is_open())
	{
		const Interaction* inter = event->Summary();
		const ProcessInfo& procInfo = inter->ProcInfo();
		LOG(debug) << "IsWeakCC " << procInfo.IsWeakCC() << "; IsWeakNC " << procInfo.IsWeakNC();

		const InitialState& initSt = inter->InitState();

		// Write the interaction info to be used in alter analysis
		outputFile << initSt.ProbePdg() 
					<< " " << initSt.ProbeE(kRfLab) 
					<< " " << procInfo.IsWeakCC() 
					<< " " << procInfo.IsWeakNC() 
					<< " " << procInfo.IsQuasiElastic()
					<< " " << procInfo.IsDeepInelastic()
					<< " " << procInfo.IsResonant()
					<< " " << procInfo.IsMEC()
					<< " " << procInfo.IsNuElectronElastic()
					<< " " << procInfo.IsElectronScattering();

		TLorentzVector* v = event->Vertex();

		outputFile << " " << v->X() << " " << v->Y()<< " " << v->Z();

		int nParticles = event->GetEntries();
		for (int i = 0; i < nParticles; i++) 
		{
			genie::GHepParticle *p = event->Particle(i);
			// kIStStableFinalState - Genie documentation: generator-level final state
			// particles to be tracked by the detector-level MC
			if ((p->Status() == genie::EGHepStatus::kIStStableFinalState)) 
			{
				if(IsPdgAllowed(p->Pdg()))
				{
					outputFile << " " << p->Pdg() << " " <<  p->Px() << " " << p->Py() << " " << p->Pz();
				}
			}
		}
		//outputFile << "EndEvent " << eventCounter++ << "=====================" << endl;
		outputFile << endl;
	}
	outputFile.close();
}

} //namespace generic 
} //namespace generators
} //namespace esbroot
