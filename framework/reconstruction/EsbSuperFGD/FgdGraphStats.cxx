#include "EsbReconstruction/EsbSuperFGD/FgdGraphStats.h"
#include "EsbReconstruction/EsbSuperFGD/FgdReconTemplate.h"
//#include "EsbData/EsbSuperFGD/FgdDetectorPoint.h"
#include "EsbDigitizer/EsbSuperFGD/FgdDigitizer.h"

// FairRoot headers
#include "FairGeoBuilder.h"
#include "FairGeoInterface.h"
#include "FairGeoLoader.h"
#include "FairGeoMedia.h"
#include "FairLogger.h"
#include <FairRootManager.h>
#include "FairVolume.h"


// Root headers
#include <TClonesArray.h>
#include <TEveManager.h>
#include <TGeoElement.h>
#include <TGeoManager.h>
#include <TFile.h>
#include <TTree.h>
#include <TDatabasePDG.h>
#include <TParticlePDG.h>

// Genie headers
#include "Framework/ParticleData/PDGCodes.h"
#include "Framework/ParticleData/PDGUtils.h"

// Genfit headers
#include "AbsBField.h"
#include "AbsMeasurement.h"
#include "ConstField.h"
#include <Exception.h>
#include <EventDisplay.h>
#include <FieldManager.h>
#include "FitStatus.h"
#include <KalmanFitterRefTrack.h>
#include "MaterialEffects.h"
#include "MeasuredStateOnPlane.h"
#include <PlanarMeasurement.h>
#include <RKTrackRep.h>
#include "SpacepointMeasurement.h"
#include <StateOnPlane.h>
#include "TDatabasePDG.h"
#include <TGeoMaterialInterface.h>
#include <Track.h>
#include <TrackCand.h>
#include <TrackPoint.h>
#include <TRandom.h>
#include <TVector3.h>



// STL headers
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <memory>
#include <math.h>
#include <bits/stdc++.h>
#include <cmath>

namespace esbroot {
namespace reconstruction {
namespace superfgd {

// -----   Default constructor   -------------------------------------------
FgdGraphStats::FgdGraphStats() : 
    FairTask() 
    , fgdConstructor("")
{
}
// -------------------------------------------------------------------------

// -----   Constructor   -------------------------------------------
FgdGraphStats::FgdGraphStats(const char* name
                          , const char* geoConfigFile
                          , const char* graphConfig
                          , const char* mediaFile
                          , const char* eventData
                          , const char* outputRootFile
                          , Int_t verbose
                          , double debugLlv) :
  FairTask(name, verbose) 
  , fgdConstructor(geoConfigFile)
  , fMinTrackLenght(0)
  , f_track_hist(nullptr)
  , freconTemplate(geoConfigFile,graphConfig)
{ 
    fpdgDB = make_shared<TDatabasePDG>();
    fParams.LoadPartParams(geoConfigFile);
}
// -------------------------------------------------------------------------



// -----   Destructor   ----------------------------------------------------
FgdGraphStats::~FgdGraphStats() 
{
    if(fHitArray) 
    {
      fHitArray->Delete();
    }

    stats.clear();
    delete fHitArray;
}
// -------------------------------------------------------------------------


InitStatus FgdGraphStats::Init()
{
  DefineMaterials();
  fsuperFgdVol = fgdConstructor.Construct();
  gGeoManager->SetTopVolume(fsuperFgdVol); 

  // Get dimentions from geometry file
  flunit = fParams.GetLenghtUnit(); // [cm]

  f_step_X  = fParams.ParamAsDouble(esbroot::geometry::superfgd::DP::length_X) * flunit;
  f_step_Y  = fParams.ParamAsDouble(esbroot::geometry::superfgd::DP::length_Y) * flunit;
  f_step_Z  = fParams.ParamAsDouble(esbroot::geometry::superfgd::DP::length_Z) * flunit;

  f_bin_X = fParams.ParamAsInt(esbroot::geometry::superfgd::DP::number_cubes_X);
  f_bin_Y = fParams.ParamAsInt(esbroot::geometry::superfgd::DP::number_cubes_Y);
  f_bin_Z = fParams.ParamAsInt(esbroot::geometry::superfgd::DP::number_cubes_Z);

  f_total_X = f_step_X * f_bin_X;
  f_total_Y = f_step_Y * f_bin_Y;
  f_total_Z = f_step_Z * f_bin_Z;

  fMinTrackLenght = fParams.ParamAsInt(esbroot::geometry::superfgd::DP::FGD_MIN_TRACK_LENGTH);

  // Get RootManager
  FairRootManager* manager = FairRootManager::Instance();
  if ( !manager ) {
    LOG(fatal) << "-E- FgdGenFitRecon::Init: " << "FairRootManager not instantised!";
    return kFATAL;
  }

  fHitArray = (TClonesArray*) manager->GetObject(geometry::superfgd::DP::FGD_HIT.c_str());
  if (!fHitArray) 
  {
      LOG(fatal) << "Exec", "No fgd hits array";
      return kFATAL;
  }

  fcanvas = new TCanvas();
  f_track_hist = new TH2F("hist_track","MC_Graph track histogram",STATS_TRACK_NUM,1,STATS_TRACK_NUM, STATS_TRACK_NUM, 1, STATS_TRACK_NUM);
  f_track_hist->GetXaxis()->SetTitle("Number of MC tracks");
  f_track_hist->GetYaxis()->SetTitle("Number Found graph tracks");

  for(size_t i=0; i<=STATS_TRACK_NUM; ++i)
  {
    stats[i]=std::vector<int>();
  }

  return kSUCCESS;
}

void FgdGraphStats::FinishTask()
{
  if(f_track_hist)
  {
    for(size_t i=1; i<=STATS_TRACK_NUM; ++i)
    {
      std::vector<int>& tr = stats[i];
      if(tr.empty())continue;

      double sum(0.0);
      int del = i == 0 ? 1 : i;
      for(size_t j=0; j<tr.size(); ++j)
      {
          //sum+= (1.0 *tr[j])/del;
          //double val = (1.0 *tr[j])/del;
          //f_track_hist->Fill(i, val);
          //f_track_hist->AddBinContent(i, val);
          f_track_hist->Fill(i, tr[j]);
      }
      
      //double avgSum = sum/tr.size();
      //if(f_track_hist) f_track_hist->Fill(i, avgSum);
      //f_track_hist->Fill(i, avgSum);
      //f_track_hist->AddBinContent(i, avgSum);
    }

    f_track_hist->Draw("colz");
    WriteCanvas("Tracks");
    f_track_hist->Reset();
  } 

  if(fcanvas)
  {
    fcanvas->ResetDrawn();
  }

  for(size_t i=0; i<=STATS_TRACK_NUM; ++i)
  {
    stats[i].clear();
  }
  
}


void FgdGraphStats::Exec(Option_t* opt)
{
    static int event = 0;
    try
    {
        LOG(info) << " Event " << event++;
        LOG(info) << " ======================== ";
        std::vector<ReconHit> allMChits;
        std::vector<std::vector<ReconHit>> foundMCTracks;

        bool rc = GetMCHits(allMChits);

        if(!rc) LOG(info) << " Could not get MC hits";

        std::vector<ReconHit> allGRhits;
        std::vector<std::vector<ReconHit*>> foundGRTracks;

        rc = rc && GetGraphHits(allGRhits);


        if(!rc) LOG(info) << " Could not get graph hits";

        if(rc)
        { 
            LOG(debug) <<" MC Hits to retrieve stats from " << allMChits.size();
            //SplitMCTrack(allMChits, foundMCTracks);
            SplitMCTrack(allGRhits, foundMCTracks);
        }
        

        if(rc)
        { 
            LOG(debug) <<" GR Hits to retrieve stats from " << allGRhits.size();
            rc = FindUsingGraph(allGRhits, foundGRTracks);
        }

        if(!rc) LOG(info) << " Could not Find graph hits";

        if(rc)
        {
            LOG(info) << "==============================";
            //LOG(info) <<" MC tracks found " << foundMCTracks.size();
            size_t numMC = 0;
            for(size_t i = 0; i < foundMCTracks.size(); ++i)
            {
                std::vector<ReconHit>& track = foundMCTracks[i];
                if(IsChargedParticle(track[0]) && (track.size() >= fMinTrackLenght) )
                {
                  ++numMC;
                  LOG(info) << " pdg " << track[0].fpdg << " length " << track.size();
                  // for(size_t j = 0; j< track.size();++j)
                  // {
                  //   ReconHit& hit = track[j];
                  //   //LOG(info) <<" xyz " << hit.fmppcLoc.X() << " " << hit.fmppcLoc.Y() << " " << hit.fmppcLoc.Z();
                  // }
                } 
            }
            LOG(info) <<" MC tracks found " << numMC;

            size_t numGRMC = 0;
            for(size_t i = 0; i < foundGRTracks.size(); ++i)
            {
                std::vector<ReconHit*>& track = foundGRTracks[i];
                if(track.size() >= fMinTrackLenght)
                {
                  ++numGRMC;
                } 
            }
            //LOG(info) <<" Graph tracks found " << foundGRTracks.size();
            LOG(info) <<" Graph tracks found " << numGRMC;
            CompareTracks(foundMCTracks, foundGRTracks);
        }
        else
        {
            LOG(error) << " Could not find clean hits or tracks! ";
        }
    }
    catch(genfit::Exception& e)
    {
        LOG(fatal) << "Exception, when tryng to fit track";
        LOG(fatal) << e.what();
    }
}

void FgdGraphStats::CompareTracks(std::vector<std::vector<ReconHit>>& mcTracks, std::vector<std::vector<ReconHit*>>& grTracks)
{
    // 1. Calculate number of trakcs in Graph
    int numOfGRTracksBigThanLimit = 0;
    for(Int_t grInd = 0 ; grInd < grTracks.size(); ++grInd)
    {
        std::vector<ReconHit*>& track = grTracks[grInd];
        if(track.size() > fMinTrackLenght) numOfGRTracksBigThanLimit++;
    }

    // 2. Calculate number of tracks in MC (charged particles only)
    int numOfMCTracksBigThanLimit = 0;
    for(Int_t mcInd = 0 ; mcInd < mcTracks.size(); ++mcInd)
    {
        std::vector<ReconHit> track = mcTracks[mcInd];
        if(track.size() > fMinTrackLenght
            && IsChargedParticle(track[0])) 
        {
           numOfMCTracksBigThanLimit++; 
        }
    }

    double ratio = (1.0 * numOfGRTracksBigThanLimit) / numOfMCTracksBigThanLimit;
    stats[numOfMCTracksBigThanLimit].push_back(numOfGRTracksBigThanLimit);
    LOG(info) << "+++++++++++++++++++++++++++++++++";
    LOG(info) << " Graph tracks ["<< fMinTrackLenght <<" cubes at least] = " << numOfGRTracksBigThanLimit;
    LOG(info) << " MC tracks ["<< fMinTrackLenght <<" cubes at least/ charge is not zero of the particle] = " << numOfMCTracksBigThanLimit;
    LOG(info) << " Number of Graph tracks / Number of MC tracks = " << ratio;
    LOG(info) << "+++++++++++++++++++++++++++++++++";

    // 3. Calculate the best match for GR track to MC track
    for(Int_t grInd = 0 ; grInd < grTracks.size(); ++grInd)
    {
        std::vector<ReconHit*>& grtrack = grTracks[grInd];
        if(grtrack.size() <= fMinTrackLenght) continue;

        Double_t maxFit = 0;
        Int_t indOfMcMaxFit = 0;

        for(Int_t mcInd = 0 ; mcInd < mcTracks.size(); ++mcInd)
        {
            std::vector<ReconHit> mctrack = mcTracks[mcInd];
            if(mctrack.size() > fMinTrackLenght
                && IsChargedParticle(mctrack[0])) 
            {
                Double_t res = CmpGrToMCTrack(mctrack, grtrack);
                if(res > maxFit) 
                {
                    maxFit = res;
                    indOfMcMaxFit = mcInd;
                }
            }
        }

        LOG(info) << "+++++++++++++++++++++++++++++++++";
        LOG(info) << " Graph track" << "[ " << grInd << " (size " << grtrack.size() << ")] best fit is " << maxFit;
        LOG(info) << " MC track length for best fit = " << mcTracks[indOfMcMaxFit].size();
        LOG(info) << "+++++++++++++++++++++++++++++++++";
    }
}

Double_t FgdGraphStats::CmpGrToMCTrack(std::vector<ReconHit>& mc, std::vector<ReconHit*>& gr)
{
    Int_t isInMC = 0;
    for(Int_t i = 0; i < gr.size(); ++i)
    {
        if(ContainsHit(mc, *gr[i]))
        {
            ++isInMC;
        } 
    }
    //LOG(info) << "Graph cube 1 =>" << (*gr[0]).fmppcLoc.X() << " " << (*gr[0]).fmppcLoc.Y() << " " << (*gr[0]).fmppcLoc.Z();
    size_t grsize = gr.size();
    //LOG(info) << "Graph cube " << grsize << " => " << (*gr[grsize-1]).fmppcLoc.X() << " " << (*gr[grsize-1]).fmppcLoc.Y() << " " << (*gr[grsize-1]).fmppcLoc.Z();
    return (1.0 * isInMC) / mc.size();
}

Bool_t FgdGraphStats::ContainsHit(std::vector<ReconHit>& mctrack, ReconHit& hit)
{
    bool rc(false); 
    for(Int_t i = 0 ; !rc && (i < mctrack.size()); ++i)
    {
        rc = (mctrack[i] == hit);
    }

    return rc;
}

Bool_t FgdGraphStats::GetMCHits(std::vector<ReconHit>& allHits)
{
  Double_t errPhotoLimit = fParams.ParamAsDouble(esbroot::geometry::superfgd::DP::FGD_ERR_PHOTO_LIMIT);

  LOG(debug) << "fHitArray->GetEntries() " << fHitArray->GetEntries();

  std::map<Long_t, Bool_t> visited;

  Int_t numVis(0);
 
  for(Int_t i =0; i <  fHitArray->GetEntries() ; i++)
  {
    data::superfgd::FgdHit* hit = (data::superfgd::FgdHit*)fHitArray->At(i);
    TVector3  photoE = hit->GetPhotoE();    
    TVector3  mppcLoc = hit->GetMppcLoc();  

    Int_t&& x = mppcLoc.X();
    Int_t&& y = mppcLoc.Y();
    Int_t&& z = mppcLoc.Z();

    //Double_t totalPhotons = photoE.X() + photoE.Y() + photoE.Z();
    //if(totalPhotons >= errPhotoLimit)
    {
      TVectorD hitPos(3);
      hitPos(0) = -f_total_X/2 + f_step_X*mppcLoc.X()  +f_step_X/2;
      hitPos(1) = -f_total_Y/2 + f_step_Y*mppcLoc.Y()  +f_step_Y/2;
      hitPos(2) = -f_total_Z/2 + f_step_Z*mppcLoc.Z()  +f_step_Z/2;

      allHits.emplace_back(ReconHit(
                                mppcLoc
                              , TVector3(hitPos(0),hitPos(1),hitPos(2))
                              , hit->GetDpos()
                              , photoE
                              , hit->GetTime()
                              , hit->GetMomentum()
                              , hit->GetExitMomentum()
                              , hit->GetTrackLenght()
                              , hit->GetTrackLengthOrigin()
                              , hit->GetPgd()
                              , hit->GetTrackId()
                              , hit->GetEdep()
                              , hit->GetPhotoDist1()
                              , hit->GetMppcDist1()
                              , hit->GetPhotoDist2()
                              , hit->GetMppcDist2()
                              , hit->GetPe()
                            )); 
    }
  }

  LOG(debug) << "allHits.size() " << allHits.size();

  return (allHits.size() > 0);
}

Bool_t FgdGraphStats::GetGraphHits(std::vector<ReconHit>& allHits)
{
  Double_t errPhotoLimit = fParams.ParamAsDouble(esbroot::geometry::superfgd::DP::FGD_ERR_PHOTO_LIMIT);

  LOG(debug) << "fHitArray->GetEntries() " << fHitArray->GetEntries();

  std::map<Long_t, Bool_t> visited;

  Int_t numVis(0);
 
  for(Int_t i =0; i <  fHitArray->GetEntries() ; i++)
  {
    data::superfgd::FgdHit* hit = (data::superfgd::FgdHit*)fHitArray->At(i);

    TVector3  photoE = hit->GetPhotoE();    
    TVector3  mppcLoc = hit->GetMppcLoc();  

    Int_t&& x = mppcLoc.X();
    Int_t&& y = mppcLoc.Y();
    Int_t&& z = mppcLoc.Z();

    LOG(debug) << "TrackId " << hit->GetTrackId();
    LOG(debug) << "GetPgd " << hit->GetPgd();
    LOG(debug) << "GetTrackLengthOrigin " << hit->GetTrackLengthOrigin();
    LOG(debug) << "GetTrackLenght " << hit->GetTrackLenght();
    
    Int_t ind = ArrInd(x,y,z);
    if(visited[ind])
    {
      // If already exists, add the photons
      continue;
    }
    visited[ind] = true;

    //Double_t totalPhotons = photoE.X() + photoE.Y() + photoE.Z();
    //if(totalPhotons >= errPhotoLimit)
    {
      TVectorD hitPos(3);
      hitPos(0) = -f_total_X/2 + f_step_X*mppcLoc.X()  +f_step_X/2;
      hitPos(1) = -f_total_Y/2 + f_step_Y*mppcLoc.Y()  +f_step_Y/2;
      hitPos(2) = -f_total_Z/2 + f_step_Z*mppcLoc.Z()  +f_step_Z/2;

      allHits.emplace_back(ReconHit(
                                mppcLoc
                              , TVector3(hitPos(0),hitPos(1),hitPos(2))
                              , hit->GetDpos()
                              , photoE
                              , hit->GetTime()
                              , hit->GetMomentum()
                              , hit->GetExitMomentum()
                              , hit->GetTrackLenght()
                              , hit->GetTrackLengthOrigin()
                              , hit->GetPgd()
                              , hit->GetTrackId()
                              , hit->GetEdep()
                              , hit->GetPhotoDist1()
                              , hit->GetMppcDist1()
                              , hit->GetPhotoDist2()
                              , hit->GetMppcDist2()
                              , hit->GetPe()
                            )); 
    }
  }

  LOG(debug) << "allHits.size() " << allHits.size();

  return (allHits.size() > 0);
}

void FgdGraphStats::SplitMCTrack(std::vector<ReconHit>& allHits, std::vector<std::vector<ReconHit>>& foundTracks)
{
  std::map<Int_t, std::vector<ReconHit>> tracks;
  for(size_t i = 0; i < allHits.size(); ++i)
  {
      ReconHit& rh = allHits[i];

      if(tracks.find(rh.ftrackId)!=tracks.end())
      {
          tracks[rh.ftrackId].push_back(rh);
      }
      else
      {
          tracks[rh.ftrackId] = std::vector<ReconHit> {rh};
          LOG(debug2) << rh.ftrackId << " " << rh.fpdg << "[pdg]";
      }
  }

  for(auto iter = tracks.begin(); iter!=tracks.end(); ++iter)
  {
      LOG(debug2) << iter->first << " track size " << (iter->second).size();
      //LOG(info) << iter->first << " track size " << (iter->second).size();
      //foundTracks.emplace_back(iter->second);
      foundTracks.push_back(iter->second);
  }

  //LOG(info) << "allHits.size() " << allHits.size();
}


void FgdGraphStats::DefineMaterials() 
{
  if(isDefinedMaterials) return; // Define materials only once

  isDefinedMaterials = true;

  new FairGeoLoader("TGeo","Geo Loader");
  FairGeoLoader *geoLoad = FairGeoLoader::Instance();
  if(geoLoad==nullptr)
  {
    LOG(error)<< "geoLoad is null";
    std::cout << "geoLoad is null" << endl;
    throw;
  }

	FairGeoInterface *geoFace = geoLoad->getGeoInterface();

  geoFace->setMediaFile(fmediaFile.c_str());
  geoFace->readMedia();

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

Bool_t FgdGraphStats::FindUsingGraph(std::vector<ReconHit>& hits
                  , std::vector<std::vector<ReconHit*>>& foundTracks)
{
  if(hits.empty())
  {
    return false;
  }

  freconTemplate.BuildGraph(hits);
  freconTemplate.FindLeaves(hits);

  // Print out the build graph
  for(Int_t i=0; i<hits.size(); ++i)
  {
    LOG(debug) << "i " << i;
    ReconHit* center = &hits[i];
    for(Int_t j=0; j<hits[i].fAllHits.size(); ++j)
    {
      ReconHit* localHit = hits[i].fAllHits[j];
      TVector3 diff = center->fmppcLoc - localHit->fmppcLoc;
      LOG(debug) << " Local Id "<< hits[i].fAllHits[j]->fLocalId << " \t X " << diff.X() << " \t Y " << diff.Y() << " \t Z " << diff.Z();
    }
    LOG(debug) << "X " << hits[i].fmppcLoc.X() << " Y " << hits[i].fmppcLoc.Y()<< " Z " << hits[i].fmppcLoc.Z();
    LOG(debug) << "Photons "<< " X " << hits[i].fphotons.X() << " Y " << hits[i].fphotons.Y()<< " Z " << hits[i].fphotons.Z();
    LOG(debug) << "=====";
  }

  std::vector<std::vector<ReconHit*>> tracks;
  freconTemplate.FindTracks(hits,tracks);

  LOG(info) <<"Leaves found " << tracks.size(); // Initially leaves are equal to the number of tracks
  LOG(debug) << "All hits -> " << hits.size(); 

  freconTemplate.CalculateGrad(tracks);

  SplitGRTrack(tracks, foundTracks);

  return !foundTracks.empty();
}

void FgdGraphStats::SplitGRTrack(std::vector<std::vector<ReconHit*>>& originalTracks, std::vector<std::vector<ReconHit*>>& splitTracks)
{
  const Double_t gradAllowedDiff = fParams.ParamAsDouble(esbroot::geometry::superfgd::DP::FGD_GRAD_ALLOWABLE_DIFF);
  LOG(debug) << "Initial tracks size " << originalTracks.size();

  for(Int_t i=0; i<originalTracks.size(); ++i)
  {
    std::vector<ReconHit*>& track = originalTracks[i];

    if(track.empty())
    {
      continue;
    }

    std::vector<ReconHit*> trackToAdd;
    trackToAdd.push_back(track[0]); // Add the first hit to start the track

    for(Int_t j=1; j<track.size(); ++j)
    {
      ReconHit* currentHit = track[j];
      ReconHit* previousHit = track[j-1];

      if(currentHit->fChangeAngle == 0)
      {
        // If there is no change add the track
        // Also '0' indicates that the track hit is too short to calculate the 
        // the change.
        trackToAdd.push_back(currentHit);
      }
      else
      {
        Double_t differenceInAngle = currentHit->fChangeAngle - previousHit->fChangeAngle;
        Double_t absDiff = std::fabs(differenceInAngle);
        if(absDiff >= gradAllowedDiff)
        {
          // If the difference is not allowed add the current track hit points
          // and reset it to start a new track.
          splitTracks.emplace_back(trackToAdd);
          trackToAdd.clear();
        }
        trackToAdd.push_back(currentHit);
      }
    }

    splitTracks.emplace_back(trackToAdd);
  }

  LOG(debug) << "Split tracks size " << splitTracks.size();
}


Bool_t FgdGraphStats::CalculateInitialMomentum(const std::vector<TVector3>& track,const TVector3& magField, TVector3& momentum , TVector3& momentumLoss)
{
  if(track.empty())
  {
      momentum.SetXYZ(0.,0.,0);
      momentumLoss.SetXYZ(0.,0.,0);
      return false;
  }

  Int_t segment = fParams.ParamAsInt(esbroot::geometry::superfgd::DP::FGD_TRACK_MOMENTUM_SEGMENT);
  Int_t avgInitialPoints = fParams.ParamAsInt(esbroot::geometry::superfgd::DP::FGD_INITIAL_TRACK_POINTS_MOMENTUM);

  const Int_t defaultSegment = 3;
  segment = (segment<=0) ? defaultSegment : segment;
  avgInitialPoints = (avgInitialPoints<=0) ? defaultSegment : avgInitialPoints;

  // If the track  lenght is less than the set segment, calculate the momentum from the arc of the whole track
  // using for the 3 points the begin, end and all the middle points between them
  Int_t lengthSize = (track.size() < segment ) ? (track.size() -1) : (segment -1) ;

  std::vector<TVector3> trackMomentums;
  std::vector<TVector3> tarckMomentumLosses;

  for(size_t i = lengthSize; i< track.size() ; ++i)
  {
    size_t p1_pos = i - lengthSize;
    TVector3 p1 = track[p1_pos];
    TVector3 p3 = track[i];

    size_t p2_pos = p1_pos + 1;
    std::vector<TVector3> segmentMomentums;

    while(p2_pos<i)
    {
      TVector3 segmentMom(0,0,0);
      TVector3 p2 = track[p2_pos];
      if(CalculateMomentum(p1, p2, p3, magField, segmentMom))
      {
        segmentMomentums.push_back(segmentMom);
      }
      ++p2_pos;
    }

    Double_t xMom(0);
    Double_t yMom(0);
    Double_t zMom(0);
    for(size_t j = 0; j< segmentMomentums.size() ; ++j)
    {
      xMom+=segmentMomentums[j].X();
      yMom+=segmentMomentums[j].Y();
      zMom+=segmentMomentums[j].Z();
    }

    xMom = xMom/segmentMomentums.size();
    yMom = yMom/segmentMomentums.size();
    zMom = zMom/segmentMomentums.size();

    trackMomentums.emplace_back(xMom, yMom, zMom);
  }

  for(size_t i = 1; i< trackMomentums.size() ; ++i)
  {
    TVector3 momLoss = trackMomentums[i] - trackMomentums[i-1];
    tarckMomentumLosses.push_back(momLoss);
  }

  Double_t xTrackMom(0);
  Double_t yTrackMom(0);
  Double_t zTrackMom(0);
  for(size_t i = 0; i < avgInitialPoints && i < trackMomentums.size() ; ++i)
  {
    xTrackMom+=trackMomentums[i].X();
    yTrackMom+=trackMomentums[i].Y();
    zTrackMom+=trackMomentums[i].Z();;
  }

  xTrackMom = xTrackMom/ avgInitialPoints;
  yTrackMom = yTrackMom/ avgInitialPoints;
  zTrackMom = zTrackMom/ avgInitialPoints;

  momentum.SetXYZ(xTrackMom, yTrackMom, zTrackMom);


  Double_t xTrackMomLoss(0);
  Double_t yTrackMomLoss(0);
  Double_t zTrackMomLoss(0);
  for(size_t i = 0; i < avgInitialPoints && i < tarckMomentumLosses.size() ; ++i)
  {
    xTrackMomLoss+=tarckMomentumLosses[i].X();
    yTrackMomLoss+=tarckMomentumLosses[i].Y();
    zTrackMomLoss+=tarckMomentumLosses[i].Z();;
  }

  xTrackMomLoss = xTrackMomLoss/ avgInitialPoints;
  yTrackMomLoss = yTrackMomLoss/ avgInitialPoints;
  zTrackMomLoss = zTrackMomLoss/ avgInitialPoints;

  momentumLoss.SetXYZ(xTrackMomLoss, yTrackMomLoss, zTrackMomLoss);


  LOG(debug3) << " =========================== Track Momentum =========================== ";
  for(size_t i = 0; i < trackMomentums.size() ; ++i)
  {
    LOG(debug3) << " Track Momentum " << "\tMag " <<  trackMomentums[i].Mag() 
                                      << "\tX " <<  trackMomentums[i].X()
                                      << "\tY " <<  trackMomentums[i].Y()
                                      << "\tZ " <<  trackMomentums[i].Z();
  }
  LOG(debug3) << " ====================================================== ";

  LOG(debug3) << " =========================== Track Momentum Loss =========================== ";
  for(size_t i = 0; i < tarckMomentumLosses.size() ; ++i)
  {
    LOG(debug3) << " Track Momentum " << "\tMag " <<  tarckMomentumLosses[i].Mag() 
                                      << "\tX " <<  tarckMomentumLosses[i].X()
                                      << "\tY " <<  tarckMomentumLosses[i].Y()
                                      << "\tZ " <<  tarckMomentumLosses[i].Z();
  }
  LOG(debug3) << " ====================================================== ";

  Double_t inf = std::numeric_limits<Double_t>::infinity();

  return (!std::isnan(momentum.Mag()) && momentum.Mag()!=inf && trackMomentums.size() > 0 );
}

Bool_t FgdGraphStats::CalculateMomentum(const TVector3& p1, const TVector3& p2, const TVector3& p3 , const TVector3& magField, TVector3& momentum)
{
  //
  //  p [Gev/c] = e [1.6 x 10^-19 coulumb] * B [T] * R [m]
  //

  Bool_t rc(false);

  // For calculation charge is taken as 1 unit of 'e'
  const Double_t charge = 1.;

  Double_t inf = std::numeric_limits<Double_t>::infinity();

  TVector3 x_axis(1,0,0);
  TVector3 y_axis(0,1,0);
  TVector3 z_axis(0,0,1);

  TVector3 length = p3 - p1;
  Double_t x_angle = x_axis.Angle(length);
  Double_t y_angle = y_axis.Angle(length);
  Double_t z_angle = z_axis.Angle(length);

  // For each magnetic field plane calculate it for the perpendicular projections
  if(magField.X()!=0)
  {
    TVector3 mag_point1(0,p1.Y(), p1.Z());
    TVector3 mag_point2(0,p2.Y(), p2.Z());
    TVector3 mag_point3(0,p3.Y(), p3.Z());
    Double_t radius = GetRadius(mag_point1,mag_point2,mag_point3); // radius is returned in [cm]

    if(!std::isnan(radius) && radius!=inf)
    {
      Double_t R = radius/100.; // convert in meters
      Double_t magField_T = magField.X() / 10.; // convert from kGauss to Tesla units
      Double_t mom = charge * R * magField_T;

      Double_t mom_x = std::cos(x_angle) * mom;
      Double_t mom_y = std::cos(y_angle) * mom;
      Double_t mom_z = std::cos(z_angle) * mom;

      momentum.SetX(mom_x);
      momentum.SetY(mom_y);
      momentum.SetZ(mom_z);
      rc = true;
    }
  }

  if(magField.Y()!=0)
  {
    TVector3 mag_point1(p1.X(), 0. , p1.Z());
    TVector3 mag_point2(p2.X(), 0. , p2.Z());
    TVector3 mag_point3(p3.X(), 0. , p3.Z());
    Double_t radius = GetRadius(mag_point1,mag_point2,mag_point3); // radius is returned in [cm]

    if(!std::isnan(radius) && radius!=inf)
    {
      Double_t R = radius/100.; // convert in meters
      Double_t magField_T = magField.Y() / 10.; // convert from kGauss to Tesla units
      Double_t mom = charge * R * magField_T;

      Double_t mom_x = std::cos(x_angle) * mom + momentum.X();
      Double_t mom_y = std::cos(y_angle) * mom + momentum.Y();
      Double_t mom_z = std::cos(z_angle) * mom + momentum.Z();

      momentum.SetX(mom_x);
      momentum.SetY(mom_y);
      momentum.SetZ(mom_z);
      rc = true;
    }
  }
  
  if(magField.Z()!=0)
  {
    TVector3 mag_point1(p1.X(), p1.Y() , 0.);
    TVector3 mag_point2(p2.X(), p2.Y() , 0.);
    TVector3 mag_point3(p3.X(), p3.Y() , 0.);
    Double_t radius = GetRadius(mag_point1,mag_point2,mag_point3); // radius is returned in [cm]

    if(!std::isnan(radius) && radius!=inf)
    {
      Double_t R = radius/100.; // convert in meters
      Double_t magField_T = magField.Z() / 10.; // convert from kGauss to Tesla units
      Double_t mom = charge * R * magField_T;

      Double_t mom_x = std::cos(x_angle) * mom + momentum.X();
      Double_t mom_y = std::cos(y_angle) * mom + momentum.Y();
      Double_t mom_z = std::cos(z_angle) * mom + momentum.Z();

      momentum.SetX(mom_x);
      momentum.SetY(mom_y);
      momentum.SetZ(mom_z);
      rc = true;
    }
  }

  return rc;
}


// Calculate the radius from 3 points using the "Menger curvature" theorem
Double_t FgdGraphStats::GetRadius(const TVector3& p1, const TVector3& p2, const TVector3& p3)
{
  //
  //          p1
  //          /\\
  //     y   /  \\  x
  //        /    \\
  //       /______\\
  //      p2        p3
  //          z
  //
  //                    2 sin (Angle(xyz))
  //      c(x,y,z) = ------------------------
  //                        |x - z|
  //
  //    where Angle(xyz) is the angle from point p2 to point p1 and p3
  //    and |x - z| is the magnitude of the line connecting points p2 and p3

  TVector3 x = p3 - p1;
  TVector3 y = p2 - p1;
  TVector3 z = p2 - p3;

  Double_t angle = y.Angle(x);
  Double_t x_z_Mag = z.Mag();

  Double_t c = (2*std::sin(angle))/x_z_Mag;
  Double_t R = 1./c;

  LOG(debug3) << "Radius is -> " << R << " [cm]";

  return R;
}

Bool_t FgdGraphStats::IsChargedParticle(ReconHit& hit)
{
    bool rc(false);

    TParticlePDG* tPdg = fpdgDB->GetParticle(hit.fpdg);
    rc = (tPdg!=nullptr) && (tPdg->Charge()!=0);

    return rc;
}

Bool_t FgdGraphStats::isParticleNeutral(Int_t pdg)
{
  Bool_t isNeutral =  (pdg ==  genie::kPdgNeutron) ||
                      (pdg ==  genie::kPdgPi0) ||
                      (pdg ==  genie::kPdgGamma) ||
                      genie::pdg::IsNeutralLepton(pdg);

  return isNeutral;
}

Bool_t FgdGraphStats::isParticleAllowed(Int_t pdg)
{
    Bool_t isAllowed =  (pdg ==  genie::kPdgPiP) ||
                        (pdg ==  genie::kPdgPiM) ||
                      genie::pdg::IsProton(pdg) ||
                      genie::pdg::IsChargedLepton(pdg);

    return isAllowed;
}

Long_t FgdGraphStats::ArrInd(int x, int y, int z)
{
  return (x*f_bin_Y*f_bin_Z + y*f_bin_Z+z);
}

void FgdGraphStats::WriteCanvas(string hist)
{
    fcanvas->Update();
    fcanvas->Draw();
    fcanvas->Write();
    std::ostringstream strb;
    strb<< "hist_" << hist << ".gif";
    fcanvas->SaveAs((strb.str()).c_str());
    fcanvas->ResetDrawn();
}

}// namespace superfgd
}// namespace reconstruction
}// namespace esbroot