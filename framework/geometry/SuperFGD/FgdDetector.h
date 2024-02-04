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
#include "geometry/SuperFGD/EsbSuperFGD/EsbSuperFGDDetectorConstruction.h"
#include "data/SuperFGD/FgdDetectorPoint.h"

#include <set>

namespace esbroot {
namespace geometry {
	class FgdDetector : public esbroot::core::detector::IDetector
	{

	  public:
		FgdDetector();
		FgdDetector(const char* geoConfigFile, double posX, double posY, double posZ);
		virtual ~FgdDetector();

		void ConstructGeometry() override;
		void GetMagneticField(Double_t& Bx,Double_t& By, Double_t& Bz);
		void GetMagneticFieldRegion(Double_t& xMin, Double_t& xMax,
                                         Double_t& yMin, Double_t& yMax,
                                         Double_t& zMin, Double_t& zMax);


		TGeoVolume* GetVolume(){return fsuperFgdVol;}
        TGeoVolume* GetSDVolume(){return fgdConstructor.GetSensitiveVolume();}

        TVector3 getDetectorPosition();

	  private:

		// Detector position
		double fposX;
		double fposY;
		double fposZ;

		TGeoVolume* fsuperFgdVol;//!<!
		esbroot::geometry::superfgd::SuperFGDDetectorConstruction    fgdConstructor;	   //! SuperFgd Detector Constructor

		FgdDetector(const FgdDetector&);
		FgdDetector& operator=(const FgdDetector&);		

		ClassDef(FgdDetector,2)
	};

} // namespace geometry
} // namespace esbroot

#endif //ESBROOT_GEOMETRY_FGDDETECTOR_H
