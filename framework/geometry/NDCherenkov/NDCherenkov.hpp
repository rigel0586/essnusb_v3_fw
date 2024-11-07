/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *              GNU Lesser General Public Licence (LGPL) version 3,             *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#ifndef ESBROOT_GEOMETRY_NDCHERENKOV_H
#define ESBROOT_GEOMETRY_NDCHERENKOV_H

#include "TClonesArray.h"
#include "TVector3.h"
#include "TLorentzVector.h"

#include "core/detector/IDetector.hpp"
#include "core/io/EsbWriterPersistency.hpp"
#include "generators/generic/IFluxNextPosition.h"

#include "G4VSensitiveDetector.hh"
#include "G4VUserDetectorConstruction.hh"

#include "core/io/EsbWriterPersistency.hpp"
#include "utility/Utility.hpp" 

#include <set>
#include <random>

namespace esbroot {
namespace geometry {

class NDCherenkov : public esbroot::core::detector::IDetector
					, public esbroot::generators::generic::IFluxNextPosition
					, public G4VSensitiveDetector
{
public:
	NDCherenkov();
	NDCherenkov(double posX, double posY, double posZ, unsigned int seed = 0);
	virtual ~NDCherenkov();

	// IDetector interface
	virtual void ConstructGeometry() override;
	virtual void PostConstructG4Geometry(G4VPhysicalVolume* G4World) override;
	virtual void AddSensitiveDetector(G4VPhysicalVolume* topVolume 
								, std::function<void(G4LogicalVolume*, G4VSensitiveDetector*)>& f_sd) override;

	void EndOfEventAction(const G4Event*) override;


	// IFluxNextPosition interface
	TVector3 NextVertexPosition() override;
	std::string GetName() override;
	//	============================

	// G4VSensitiveDetector
	virtual void    Initialize(G4HCofThisEvent*) override {};
  	virtual G4bool  ProcessHits(G4Step* astep,G4TouchableHistory* ROHist) override;
  	virtual void    EndOfEvent(G4HCofThisEvent*) override {};
	// ============================
private:

	const std::string fName{"WaterWithGd"};
	// Detector position
	double fposX;
	double fposY;
	double fposZ;	

	G4double fwater_radius{0};

	utility::Utility fut;
	core::io::WriterInfo  fDataPointCollection;  //! 

	std::mt19937 frndGen;//!<!
    std::uniform_real_distribution<Double_t> fdis;//!<!

	ClassDef(NDCherenkov,2)
};

} // namespace geometry
} // namespace esbroot

#endif //ESBROOT_GEOMETRY_NDCHERENKOV_H
