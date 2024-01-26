/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *              GNU Lesser General Public Licence (LGPL) version 3,             *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#include "EsbGeometry/FgdDetector.h"
#include "EsbGeometry/EsbSuperFGD/Materials.h"
#include "EsbGeometry/EsbSuperFGD/Names.h"
#include "EsbGeometry/EsbSuperFGD/EsbFgdDetectorParameters.h" 

#include "FairLogger.h"
#include "FairGenericStack.h"
#include "FairGeoLoader.h"
#include "FairGeoInterface.h"
#include "FairGeoBuilder.h"
#include "FairGeoMedia.h"
#include "FairRootManager.h"
#include "FairStack.h"
#include "FairVolume.h"

#include "TClonesArray.h"
#include "TVirtualMC.h"
#include "TGeoManager.h"
#include "TGeoMaterial.h"
#include "TGeoMedium.h"
#include "TGraph.h"

#include "Geant4GM/volumes/Factory.h"
#include "RootGM/volumes/Factory.h"
#include "TGeoManager.h"

#include <iostream>
using std::cout;
using std::endl;


#include "TGeoTube.h"

namespace esbroot {

namespace geometry {
// PC: work around
static const Int_t kFgdDetector = 1;

//___________________________________________________________________
FgdDetector::FgdDetector()
  : FairDetector("FgdDetector", kTRUE, kFgdDetector),
    fgdConstructor(""),
    fTrackID(-1),
    fVolumeID(-1),
    fPos(),
    fMom(),
    fTime(-1.),
    fLength(-1.),
    fELoss(-1),
    fposX(0),
    fposY(0),
    fposZ(0),
    isDefinedMaterials(false),
    fFgdDetectorPointCollection(new TClonesArray(data::superfgd::FgdDetectorPoint::Class())),
    fsuperFgdVol(nullptr)
{
}

FgdDetector::FgdDetector(const char* geoConfigFile, double posX, double posY, double posZ, Bool_t Active)
  : FairDetector("FgdDetector", kTRUE, kFgdDetector),
    fgdConstructor(geoConfigFile),
    fTrackID(-1),
    fVolumeID(-1),
    fPos(),
    fMom(),
    fMomExit(),
    fTime(-1.),
    fLength(-1.),
    fELoss(-1),
    fposX(posX),
    fposY(posY),
    fposZ(posZ),
    isDefinedMaterials(false),
    fFgdDetectorPointCollection(new TClonesArray(data::superfgd::FgdDetectorPoint::Class())),
    fsuperFgdVol(nullptr)
{
}

//___________________________________________________________________
  FgdDetector::FgdDetector(const char* name, const char* geoConfigFile, double posX, double posY, double posZ, Bool_t active)
  : FairDetector(name, active, kFgdDetector),
    fgdConstructor(geoConfigFile),
    fTrackID(-1),
    fVolumeID(-1),
    fPos(),
    fMom(),
    fMomExit(),
    fTime(-1.),
    fLength(-1.),
    fELoss(-1),
    fposX(posX),
    fposY(posY),
    fposZ(posZ),
    isDefinedMaterials(false),
    fFgdDetectorPointCollection(new TClonesArray(data::superfgd::FgdDetectorPoint::Class())) 
{
}

//___________________________________________________________________
FgdDetector::~FgdDetector()
{
  if (fFgdDetectorPointCollection) {
    fFgdDetectorPointCollection->Delete();
    delete fFgdDetectorPointCollection;
  }
}

//___________________________________________________________________
void FgdDetector::Initialize()
{
  FairDetector::Initialize();
  fpdgCodes.clear();
}

//___________________________________________________________________
Bool_t  FgdDetector::ProcessHits(FairVolume* vol)
{
  if ( TVirtualMC::GetMC()->IsTrackEntering() ) {
    fELoss  = 0.;
    fLength = 0.;
    fTime   = TVirtualMC::GetMC()->TrackTime() * 1.0e09;
    TVirtualMC::GetMC()->TrackPosition(fPos);
    TVirtualMC::GetMC()->TrackMomentum(fMom);
  }

  // Sum energy loss for all steps in the active volume
  fELoss += TVirtualMC::GetMC()->Edep();
  fLength += TVirtualMC::GetMC()->TrackStep();

  // Create FairTutorialDet1Point at exit of active volume
  if ( TVirtualMC::GetMC()->IsTrackExiting()
      //  || TVirtualMC::GetMC()->IsTrackStop()
      //  || TVirtualMC::GetMC()->IsTrackDisappeared()
       ) {

    fTrackID  = TVirtualMC::GetMC()->GetStack()->GetCurrentTrackNumber();
    fVolumeID = vol->getMCid();

    //~ if (fELoss == 0. ) { return kFALSE; }
    TVirtualMC::GetMC()->TrackPosition(fPosExit);
    TVirtualMC::GetMC()->TrackMomentum(fMomExit);

    LOG(debug) << "  TrackPid " << TVirtualMC::GetMC()->TrackPid();
    LOG(debug) << "  TrackCharge " << TVirtualMC::GetMC()->TrackCharge();
    LOG(debug) << "  Is track entering " << TVirtualMC::GetMC()->IsTrackEntering();
    LOG(debug) << "  Is track exiting " << TVirtualMC::GetMC()->IsTrackExiting();
    LOG(debug) << "  Is track disappearing " << TVirtualMC::GetMC()->IsTrackDisappeared();
    LOG(debug) << "  vol->getCopyNo() " << vol->getCopyNo();
    LOG(debug) << "  vol->getVolumeId() " << vol->getVolumeId();
    LOG(debug) << "  fPos.X() " << fPos.X();
    LOG(debug) << "  fPos.Y() " << fPos.Y();
    LOG(debug) << "  fPos.Z() " << fPos.Z();
    LOG(debug) << "  TrackLength " << TVirtualMC::GetMC()->TrackLength();
    LOG(debug) << "  GetCurrentTrackNumber " << TVirtualMC::GetMC()->GetStack()->GetCurrentTrackNumber();
    LOG(debug) << "  TrackPid " << TVirtualMC::GetMC()->TrackPid();

    AddHit(fTrackID, fVolumeID
          ,TVector3(fposX,       fposY,       fposZ)
          ,TVector3(fPos.X(),       fPos.Y(),       fPos.Z())
          ,TVector3(fPosExit.X(),   fPosExit.Y(),   fPosExit.Z())
          ,TVector3(fMom.Px(),      fMom.Py(),      fMom.Pz())
          ,TVector3(fMomExit.Px(),      fMomExit.Py(),      fMomExit.Pz())
          ,fTime, fELoss, fLength, TVirtualMC::GetMC()->TrackPid()
          , TVirtualMC::GetMC()->TrackLength()); 

    fpdgCodes.clear();
  }    

  return kTRUE;
}

void FgdDetector::EndOfEvent()
{
  fFgdDetectorPointCollection->Clear();
}



void FgdDetector::Register()
{

  /** This will create a branch in the output tree called
      EsbFgdDetectorPoint, setting the last parameter to kFALSE means:
      this collection will not be written to the file, it will exist
      only during the simulation.
  */
  
  FairRootManager::Instance()->Register(superfgd::DP::FGD_BRANCH.c_str()
                                        ,superfgd::DP::FGD_DETECTOR_NAME.c_str()
                                        ,fFgdDetectorPointCollection, kTRUE);

}


TClonesArray* FgdDetector::GetCollection(Int_t iColl) const
{
  if (iColl == 0) { return fFgdDetectorPointCollection; }
  else { return NULL; }
}

void FgdDetector::Reset()
{
  fFgdDetectorPointCollection->Clear();
}

void FgdDetector::ConstructGeometry()
{
  using namespace geometry::superfgd;

  DefineMaterials();
  // Create the real Fgd geometry
  TGeoVolume* superFgdVol = fgdConstructor.Construct();
  fsuperFgdVol = superFgdVol;
  // Retrieve the sensitive volume
  TGeoVolume* cubeScnintilatorVol = fgdConstructor.GetSensitiveVolume();

  if(!superFgdVol || !cubeScnintilatorVol)
  {
    throw "SuperFGD was not constructed successfully!";
  }

  // Register sensitive volume
  AddSensitiveVolume(cubeScnintilatorVol);

  TGeoVolume *top = gGeoManager->GetTopVolume();
  top->AddNode(superFgdVol, 1, new TGeoTranslation(fposX, fposY, fposZ));
}

//___________________________________________________________________
data::superfgd::FgdDetectorPoint* FgdDetector::AddHit(Int_t trackID, Int_t detID, 
					  TVector3 detectorPos, TVector3 pos , TVector3 posExit, TVector3 mom,
					  TVector3 momExit , Double32_t time, Double32_t edep, Double32_t trackLength, Int_t pdg
            , Double32_t trackLengthFromOrigin )
{
    LOG(debug) << "FgdDetector::AddHit";
    LOG(debug) << "trackID " << trackID;
    LOG(debug) << "detID " << detID;
    LOG(debug) << "pos.X() " << pos.X() << "; pos.Y() " << pos.Y()<< "; pos.Z() " << pos.Z();
    LOG(debug) << "mom.Px() " << mom.Px() << "; mom.Py() " << mom.Py() << "; mom.Pz() " << mom.Pz();
    LOG(debug) << "time " << time;
    LOG(debug) << "edep " << edep;
    LOG(debug) << "pdg " << pdg;

  TClonesArray& clref = *fFgdDetectorPointCollection;
  Int_t size = clref.GetEntriesFast();

  return new(clref[size]) data::superfgd::FgdDetectorPoint(trackID, detID, detectorPos, pos, posExit, mom, 
					     momExit, time, edep, trackLength, pdg, trackLengthFromOrigin);
}

void  FgdDetector::SetSpecialPhysicsCuts()
{
  // DRAY - delta ray production. The variable DRAY controls this process.
  // 0 - No delta rays production.
  // 1 - delta rays production with generation of . Default setting.
  // 2 - delta rays production without generation of .
  // LOSS - Continuous energy loss. The variable LOSS controls this process. 
  // 0 - No continuous energy loss, DRAY is set to 0.
  // 1 - Continuous energy loss with generation of delta rays above DCUTE and restricted Landau fluctuations below DCUTE. 
  // 2 - Continuous energy loss without generation of delta rays and full Landau-Vavilov-Gauss fluctuations. In this case the variable DRAY is forced to 0 to avoid double counting of fluctuations. Default setting. 
  // 3 - Same as 1, kept for backward compatibility. 
  // 4 - Energy loss without fluctuation. The value obtained from the tables is used directly. 
  TVirtualMC::GetMC()->SetProcess("LOSS",2);  /**energy loss*/
  TVirtualMC::GetMC()->SetProcess("DRAY",0); /**delta-ray*/

  Double_t cut_delta = 1.0E4; // GeV --> 10 TeV
  TVirtualMC::GetMC()->SetCut("DCUTE", cut_delta); /** delta-rays by electrons */
  TVirtualMC::GetMC()->SetCut("DCUTM", cut_delta); /** delta-rays by muons */ 
}

void FgdDetector::DefineMaterials() 
{
  if(isDefinedMaterials) return; // Define materials only once

  isDefinedMaterials = true;

  FairGeoLoader *geoLoad = FairGeoLoader::Instance();
	FairGeoInterface *geoFace = geoLoad->getGeoInterface();
	
	FairGeoMedia *geoMedia = geoFace->getMedia();
	FairGeoBuilder* geoBuild = geoLoad->getGeoBuilder();

  // FairGeoMedium* brass = geoMedia->getMedium(esbroot::geometry::superfgd::materials::brass);
	// geoBuild->createMedium(brass);

  // FairGeoMedium* bronze = geoMedia->getMedium(esbroot::geometry::superfgd::materials::bronze);
	// geoBuild->createMedium(bronze);

  // FairGeoMedium* stainlessSteel = geoMedia->getMedium(esbroot::geometry::superfgd::materials::stainlessSteel);
	// geoBuild->createMedium(stainlessSteel);

  // FairGeoMedium* methane = geoMedia->getMedium(esbroot::geometry::superfgd::materials::methane);
	// geoBuild->createMedium(methane);

  // FairGeoMedium* carbonDioxide = geoMedia->getMedium(esbroot::geometry::superfgd::materials::carbonDioxide);
	// geoBuild->createMedium(carbonDioxide);

  // FairGeoMedium* carbontetraFloride = geoMedia->getMedium(esbroot::geometry::superfgd::materials::carbontetraFloride);
	// geoBuild->createMedium(carbontetraFloride);

  // FairGeoMedium* titaniumDioxide = geoMedia->getMedium(esbroot::geometry::superfgd::materials::titaniumDioxide);
	// geoBuild->createMedium(titaniumDioxide);

  // FairGeoMedium* polystyrene = geoMedia->getMedium(esbroot::geometry::superfgd::materials::polystyrene);
	// geoBuild->createMedium(polystyrene);

  FairGeoMedium* scintillator = geoMedia->getMedium(esbroot::geometry::superfgd::materials::scintillator);
  scintillator->setMediumIndex(esbroot::geometry::superfgd::materials::GetNextIndex());
	geoBuild->createMedium(scintillator);
  scintillator->Print();

  FairGeoMedium* paraterphnyl = geoMedia->getMedium(esbroot::geometry::superfgd::materials::paraterphnyl);
	geoBuild->createMedium(paraterphnyl);

  // FairGeoMedium* podscintillator = geoMedia->getMedium(esbroot::geometry::superfgd::materials::podscintillator);
	// geoBuild->createMedium(podscintillator);

  // FairGeoMedium* polyethylene = geoMedia->getMedium(esbroot::geometry::superfgd::materials::polyethylene);
	// geoBuild->createMedium(polyethylene);

  // FairGeoMedium* poduleEpoxy = geoMedia->getMedium(esbroot::geometry::superfgd::materials::poduleEpoxy);
	// geoBuild->createMedium(poduleEpoxy);

  // FairGeoMedium* polycarbonate = geoMedia->getMedium(esbroot::geometry::superfgd::materials::polycarbonate);
	// geoBuild->createMedium(polycarbonate);

  // FairGeoMedium* carbonFiber = geoMedia->getMedium(esbroot::geometry::superfgd::materials::carbonFiber);
	// geoBuild->createMedium(carbonFiber);

  FairGeoMedium* fiberCore = geoMedia->getMedium(esbroot::geometry::superfgd::materials::fiberCore);
	geoBuild->createMedium(fiberCore);

  FairGeoMedium* fiberCladding = geoMedia->getMedium(esbroot::geometry::superfgd::materials::fiberCladding);
	geoBuild->createMedium(fiberCladding);

  FairGeoMedium* fairTiO2 = geoMedia->getMedium(esbroot::geometry::superfgd::materials::titaniumDioxide);
  geoBuild->createMedium(fairTiO2);

  FairGeoMedium* fairPolystyrene = geoMedia->getMedium(esbroot::geometry::superfgd::materials::polystyrene);
  geoBuild->createMedium(fairPolystyrene);

  FairGeoMedium* fairAir = geoMedia->getMedium(esbroot::geometry::superfgd::materials::air);
  geoBuild->createMedium(fairAir);

  FairGeoMedium* vacuum = geoMedia->getMedium(esbroot::geometry::superfgd::materials::vacuum);
  geoBuild->createMedium(vacuum);
}


void FgdDetector::GetMagneticField(Double_t& Bx,Double_t& By, Double_t& Bz)
{
  TVector3 magField = fgdConstructor.GetMagneticField();
  Bx = magField.X();
  By = magField.Y();
  Bz = magField.Z();
}


void FgdDetector::GetMagneticFieldRegion(Double_t& xMin, Double_t& xMax,
                                         Double_t& yMin, Double_t& yMax,
                                         Double_t& zMin, Double_t& zMax)
{
  Double_t edge = fgdConstructor.GetEdge() * fgdConstructor.GetLengthUnit();
  Double_t totalX = fgdConstructor.GetCubeNX() * edge;
  Double_t totalY = fgdConstructor.GetCubeNY() * edge;
  Double_t totalZ = fgdConstructor.GetCubeNZ() * edge;

  xMin = fposX - totalX/2;
  xMax = fposX + totalX/2;

  yMin = fposY - totalY/2;
  yMax = fposY + totalY/2;

  zMin = fposZ - totalZ/2;
  zMax = fposZ + totalZ/2;
}

}
}

