/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *              GNU Lesser General Public Licence (LGPL) version 3,             *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#ifndef ESBROOT_GEOMETRY_FGDDETECTOR_H
#define ESBROOT_GEOMETRY_FGDDETECTOR_H

#include "TVector3.h"
#include "TLorentzVector.h"

#include "core/detector/IDetector.hpp"
#include "geometry/SuperFGD/EsbSuperFGD/SuperFGDDetectorConstruction.h"
#include "data/SuperFGD/FgdDetectorPoint.hpp"

#include "G4VSensitiveDetector.hh"
#include "G4VUserDetectorConstruction.hh"

#include <set>

namespace esbroot {
namespace geometry {

class FgdDetector : public esbroot::core::detector::IDetector,
					public G4VSensitiveDetector
{
public:
	FgdDetector();
	FgdDetector(const char* geoConfigFile, double posX, double posY, double posZ);
	virtual ~FgdDetector();

	void ConstructGeometry() override;
	void AddSensitiveDetector(G4VPhysicalVolume* topVolume 
								, std::function<void(G4LogicalVolume*, G4VSensitiveDetector*)>& f_sd) override;

	void GetMagneticField(Double_t& Bx,Double_t& By, Double_t& Bz);
	void GetMagneticFieldRegion(Double_t& xMin, Double_t& xMax,
										Double_t& yMin, Double_t& yMax,
										Double_t& zMin, Double_t& zMax);


	TGeoVolume* GetVolume(){return fsuperFgdVol;}
	TGeoVolume* GetSDVolume(){return fgdConstructor.GetSensitiveVolume();}

	// G4VSensitiveDetector
	void    Initialize(G4HCofThisEvent*);
  	G4bool  ProcessHits(G4Step* astep,G4TouchableHistory* ROHist);
  	void    EndOfEvent(G4HCofThisEvent*);
	// ============================

	TVector3 getDetectorPosition();

private:

	// Detector position
	double fposX;
	double fposY;
	double fposZ;

	std::string fCubeName{""};

	/** Track information to be stored until the track leaves the
	active volume.
	*/
	Int_t          fTrackID;           //!  track index
	Int_t          fVolumeID;          //!  volume id
	std::string    fVolumeName{""};          //!  volume id
	TLorentzVector fPos;               //!  position at entrance
	TLorentzVector fPosExit;           //!  position at exit
	TLorentzVector fMom;               //!  momentum at entrance
	TLorentzVector fMomExit;               //!  momentum at entrance
	Double32_t     fTime;              //!  time
	Double32_t     fLength;            //!  length
	Double32_t     fELoss;             //!  energy loss

	TGeoVolume* fsuperFgdVol;//!<!
	esbroot::geometry::superfgd::SuperFGDDetectorConstruction    fgdConstructor;	   //! SuperFgd Detector Constructor

	FgdDetector(const FgdDetector&);
	FgdDetector& operator=(const FgdDetector&);		

	void AddMagneticField(G4VPhysicalVolume* detectorPhVol);

	ClassDef(FgdDetector,2)
};

} // namespace geometry
} // namespace esbroot

#endif //ESBROOT_GEOMETRY_FGDDETECTOR_H
