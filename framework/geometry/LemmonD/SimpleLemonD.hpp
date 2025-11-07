/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *              GNU Lesser General Public Licence (LGPL) version 3,             *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#ifndef ESBROOT_GEOMETRY_SimpleLemonD_H
#define ESBROOT_GEOMETRY_SimpleLemonD_H

#include "TObject.h"
#include "TGeoVolume.h"
#include "generators/generic/IFluxNextGenerator.h"
#include "core/detector/IDetector.hpp"
#include "G4VSensitiveDetector.hh"

namespace esbroot {
namespace geometry {

class SimpleLemonD : public TObject
						 , public generators::generic::IFluxNextGenerator
						 , public esbroot::core::detector::IDetector
						 , public G4VSensitiveDetector
{
public:
	struct NuFileEntry
	{
		int pdgNu;
		double momX;
		double momY;
		double momZ;

		double posX;
		double posY;
		double posZ;

		double radius;
	};

	SimpleLemonD(const std::string& outnuFile, const std::string& eventWriteFile);

	virtual ~SimpleLemonD();

	TGeoVolume* getVolume(){return fLemonDWater;}

	// IFluxNextGenerator
	bool NextPosMomPdg(TVector3& position, TLorentzVector& momentum, int& pdgCode) override;
	void WriteEvent(const genie::EventRecord *event) override;

	const std::vector<NuFileEntry>& getNuEntries() {return fnuEntries;}


	// IDetector
	void ConstructGeometry() override;

	// G4VSensitiveDetector
	void    Initialize(G4HCofThisEvent*) override {};
  	G4bool  ProcessHits(G4Step* astep,G4TouchableHistory* ROHist) {return true;}
  	void    EndOfEvent(G4HCofThisEvent*) override {};

	std::string GetName(){ return fName; }

private:
	TGeoVolume* fLemonDWater{nullptr};

	void init();
	void ReadNuFluxFile(const char* fluxFile);

	std::vector<NuFileEntry> fnuEntries;
	size_t fCounter{0};
	Int_t fPdgNu;
	const std::string fName = "SimpleLemonD";
	const std::string fEventWriteFile;
	ClassDef(SimpleLemonD,2)
};

} // namespace geometry
} // namespace esbroot

#endif //ESBROOT_GEOMETRY_SimpleLemonD_H
