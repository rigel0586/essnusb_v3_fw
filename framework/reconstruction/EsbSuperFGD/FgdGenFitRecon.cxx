#include "EsbReconstruction/EsbSuperFGD/FgdGenFitRecon.h"
#include "EsbReconstruction/EsbSuperFGD/FgdReconTemplate.h"
#include "EsbData/EsbSuperFGD/FgdDetectorPoint.h"

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


// Genfit headers
#include "AbsBField.h"
#include "AbsMeasurement.h"
#include "ConstField.h"
#include <Exception.h>
#include <EventDisplay.h>
#include <FieldManager.h>
#include "FitStatus.h"
#include <KalmanFitterRefTrack.h>
#include <KalmanFitter.h>
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


// PathFinder headers
#include "FinderParameter.h"
#include "HoughTrafoTrackFinder.h"
#include "TrackParameterFull.h"


// STL headers
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <memory>
#include <math.h>
#include <bits/stdc++.h>

namespace esbroot {
namespace reconstruction {
namespace superfgd {

// -----   Default constructor   -------------------------------------------
FgdGenFitRecon::FgdGenFitRecon() :
  FairTask(), fsuperFgdVol(nullptr)
  , fgdConstructor("")
  , fHitArray(nullptr)
  , isDefinedMaterials(false)
  , fDebuglvl_genfit(0)
  , fmediaFile("")
  , fTracksArray(nullptr)
  , fdisplay(nullptr)
  , isGenFitVisualization(false)
  , fGenFitVisOption("")
  , fminGenFitInterations(2)
  , fmaxGenFitIterations(4)
  , fminHits(25)
  , ffinder(TrackFinder::HOUGH_PATHFINDER_CURL)
{ 
}
// -------------------------------------------------------------------------

// -----   Constructor   -------------------------------------------
FgdGenFitRecon::FgdGenFitRecon(const char* name
                          , const char* geoConfigFile
                          , const char* graphConfig
                          , const char* mediaFile
                          , Int_t verbose
                          , double debugLlv
                          , bool visualize
                          , std::string visOption) :
  FairTask(name, verbose)
  , fsuperFgdVol(nullptr)
  , fgdConstructor(geoConfigFile)
  , fHitArray(nullptr)
  , isDefinedMaterials(false)
  , fDebuglvl_genfit(debugLlv)
  , fmediaFile(mediaFile)
  , fTracksArray(nullptr)
  , fdisplay(nullptr)
  , isGenFitVisualization(visualize)
  , fGenFitVisOption(visOption)
  , fminGenFitInterations(2)
  , fmaxGenFitIterations(4)
  , fminHits(25)
  , ffinder(TrackFinder::HOUGH_PATHFINDER_CURL)
  , freconTemplate(geoConfigFile, graphConfig)
{ 
  fParams.LoadPartParams(geoConfigFile);
}
// -------------------------------------------------------------------------



// -----   Destructor   ----------------------------------------------------
FgdGenFitRecon::~FgdGenFitRecon() 
{
  if(fHitArray) {
    fHitArray->Delete();
    delete fHitArray;
  }

  if(fTracksArray) {
    fTracksArray->Delete();
    delete fTracksArray;
  }
}
// -------------------------------------------------------------------------



// -----   Public method Init   --------------------------------------------
InitStatus FgdGenFitRecon::Init() 
{   
  // Create the real Fgd geometry
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

  OutputFileInit(manager);

  if(isGenFitVisualization)
  {
    fdisplay = genfit::EventDisplay::getInstance();
  }
  
  if(fdisplay!=nullptr && !fGenFitVisOption.empty())
  {
    fdisplay->setOptions(fGenFitVisOption);
  }
  

  return kSUCCESS;
}

void FgdGenFitRecon::OutputFileInit(FairRootManager* manager)
{
  // Create and register output array
  fTracksArray = new TClonesArray(genfit::Track::Class(), 1000);
  manager->Register(geometry::superfgd::DP::FGD_FIT_TRACK.c_str()
                    , geometry::superfgd::DP::FGD_BRANCH_FIT.c_str()
                    , fTracksArray, kTRUE);
}


// -------------------------------------------------------------------------



// -----   Public methods   --------------------------------------------
void FgdGenFitRecon::FinishEvent()
{
  
}

void FgdGenFitRecon::FinishTask()
{
  if(isGenFitVisualization)
  {
    fdisplay->open();
  }
}

void FgdGenFitRecon::Exec(Option_t* opt) 
{  
  try
  {
    std::vector<ReconHit> allhits;
    std::vector<std::vector<TVector3>> foundTracks;
    std::vector<std::vector<ReconHit*>> reconTracks;

    bool rc = GetHits(allhits);
    if(rc)
    { 
      LOG(debug) <<" Hits to retrieve tracks from " << allhits.size();
      switch(ffinder)
      {
        case TrackFinder::HOUGH_PATHFINDER_LINE:
                                          rc = FindUsingHough(allhits, foundTracks, FgdReconTemplate::HoughType::STRAIGHT_LINE);
                                          break;
        case TrackFinder::HOUGH_PATHFINDER_HELIX:
                                          rc = FindUsingHough(allhits, foundTracks, FgdReconTemplate::HoughType::HELIX);
                                          break;
        case TrackFinder::HOUGH_PATHFINDER_CURL:
                                          rc = FindUsingHough(allhits, foundTracks, FgdReconTemplate::HoughType::CURL);
                                          break;
        case TrackFinder::GRAPH:
                                          rc = FindUsingGraph(allhits, foundTracks, reconTracks);
                                          break;
        case TrackFinder::GRAPH_HOUGH_PATHFINDER:
                                          rc = FindUsingGraphHough(allhits, foundTracks);
                                          break;
        case TrackFinder::HOUGH_GRAPH_LEAVES:
                                          rc = FindUsingHoughGraphLeaves(allhits, foundTracks);
                                          break;   
        default: 
                rc = false;
                break;
      }
    }

    if(rc)
    {
      LOG(debug) <<" Tracks found " << foundTracks.size();
      FitTracks(foundTracks);
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
// -------------------------------------------------------------------------


// -----   Protected methods   --------------------------------------------

Bool_t FgdGenFitRecon::GetHits(std::vector<ReconHit>& allHits)
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
      // ReconHit toFind;
      // toFind.fmppcLoc = mppcLoc;
      // std::vector<ReconHit>::iterator recHit = find(allHits.begin(), allHits.end(), toFind);
      // ReconHit& foundHit = *recHit;
      // foundHit.fphotons = foundHit.fphotons + photoE;
      continue;
    }
    visited[ind] = true;

    Double_t totalPhotons = photoE.X() + photoE.Y() + photoE.Z();
    if(totalPhotons >= errPhotoLimit)
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

Bool_t FgdGenFitRecon::FindUsingHough(std::vector<ReconHit>& hits
                                , std::vector<std::vector<TVector3>>& foundTracks
                                , FgdReconTemplate::HoughType houghType)
{
  LOG(debug) << "hits " << hits.size();

  unsigned int use_vertex = fParams.ParamAsDouble(esbroot::geometry::superfgd::DP::PF_USE_VERTEX);
  bool useVerex= (use_vertex == 1);
  double vertexX = fParams.ParamAsDouble(esbroot::geometry::superfgd::DP::PF_VERTEXX);
  double vertexY = fParams.ParamAsDouble(esbroot::geometry::superfgd::DP::PF_VERTEXY);
  TVector3 vertex(vertexX, vertexY, 0);
  std::vector<std::vector<ReconHit*>> reconHitTracks;

  bool found = freconTemplate.FindUsingHough(hits
                                , reconHitTracks
                                , foundTracks
                                , houghType
                                , vertex
                                , useVerex);

  return found;
}

Bool_t FgdGenFitRecon::FindUsingGraph(std::vector<ReconHit>& hits
                  , std::vector<std::vector<TVector3>>& foundTracks
                  , std::vector<std::vector<ReconHit*>>& reconTracks)
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
  

  LOG(debug) <<"Leaves found " << tracks.size(); // Initially leaves are equal to the number of tracks

  freconTemplate.CalculateGrad(tracks);
  freconTemplate.SplitTrack(tracks, reconTracks);

  Int_t totalHitsInTracks(0);
  for(size_t i = 0; i<reconTracks.size(); ++i)
  {
    std::vector<ReconHit*>& track = reconTracks[i];
    std::vector<TVector3> currentTrack;
    LOG(debug2) << "=============================== ";
    LOG(debug2) << "Track " << i;
    
    totalHitsInTracks+=track.size();

    for(size_t j = 0; j<track.size(); ++j)
    {
      ReconHit* trackHit = track[j];

      //if(j==0) // first hit
      {
        LOG(debug2) << "Pdg code particle " << trackHit->fpdg;
        LOG(debug2) << "Particle momentum " << trackHit->fmom.Mag();
        LOG(debug2) << " \tmomX " << trackHit->fmom.X() << " \tmomY " << trackHit->fmom.Y() << " \tmomZ " << trackHit->fmom.Z();
      }

      currentTrack.emplace_back(trackHit->fHitPos.X()
                              , trackHit->fHitPos.Y()
                              , trackHit->fHitPos.Z());   

      LOG(debug2) << " \tX " << trackHit->fmppcLoc.X() << " \tY " << trackHit->fmppcLoc.Y() << " \tZ " << trackHit->fmppcLoc.Z();    

      // if(j== (track.size() -1 )) // last hit
      // {
      //   LOG(debug2) << "Pdg code particle " << trackHit->fpdg;
      //   LOG(debug2) << "Particle momentum " << trackHit->fmom.Mag();
      //   LOG(debug2) << " \tmomX " << trackHit->fmom.X() << " \tmomY " << trackHit->fmom.Y() << " \tmomZ " << trackHit->fmom.Z();
      // }
    }
    LOG(debug2) << "=============================== ";

    foundTracks.push_back(currentTrack);
  }

  LOG(debug) << "Total hits in tracks " << totalHitsInTracks;

  return !foundTracks.empty();
}

Bool_t FgdGenFitRecon::FindUsingGraphHough(std::vector<ReconHit>& hits
                  , std::vector<std::vector<TVector3>>& foundTracks)
{
  
  Bool_t rc(false);

  std::vector<std::vector<TVector3>> graphTracks;
  std::vector<std::vector<ReconHit*>> reconTracks;
  rc = FindUsingGraph(hits, graphTracks,reconTracks);

  std::vector<ReconHit> graphFoundHits;
  ReconHit hit;
  for(size_t i = 0; i < graphTracks.size(); ++i)
  {
    std::vector<TVector3>& grtr = graphTracks[i];
    for(size_t j = 0; j < grtr.size(); ++j)
    {
      hit.fHitPos = grtr[j];
      graphFoundHits.push_back(hit);
    }
  }

  if(rc)
  {
    // Traverse through the found graph tracks and use hough transform for each track
    // add the found hough track with the biggest number of found hits
    for(size_t i = 0; i < graphTracks.size(); ++i)
    {
      size_t biggestSize(0);
      std::vector<TVector3>* trackToAdd = nullptr;

      // 1. Line hough ========================
      std::vector<std::vector<TVector3>> hough_Line;
      if(FindUsingHough(graphFoundHits, hough_Line, FgdReconTemplate::HoughType::STRAIGHT_LINE)
          && !hough_Line.empty())
      {
        size_t localBiggest(0);
        size_t ind(0);
        for(size_t j = 0; j < hough_Line.size(); ++j)
        {
          if(localBiggest < hough_Line[j].size())
          {
            localBiggest = hough_Line[j].size();
            ind = j;
          }
        }

        if(localBiggest > biggestSize)
        {
          biggestSize = hough_Line[ind].size();
          trackToAdd = &hough_Line[ind];
          LOG(debug) << "Hough transform from graph [line] size of track is: " << hough_Line[ind].size();
        }
      }
      LOG(debug)<< "Hough transform [line] " << hough_Line.size();

      // ======================================

      // 2. Helix hough ========================
      std::vector<std::vector<TVector3>> hough_Helix;
      if(FindUsingHough(graphFoundHits, hough_Helix, FgdReconTemplate::HoughType::HELIX)
        && !hough_Helix.empty()) 
      {
        size_t localBiggest(0);
        size_t ind(0);
        for(size_t j = 0; j < hough_Helix.size(); ++j)
        {
          if(localBiggest < hough_Helix[j].size())
          {
            localBiggest = hough_Helix[j].size();
            ind = j;
          }
        }

        if(localBiggest > biggestSize)
        {
          biggestSize = hough_Helix[ind].size();
          trackToAdd = &hough_Helix[ind];
          LOG(debug) << "Hough transform from graph [helix] size of track is: " << hough_Helix[ind].size();
        }
      }
      LOG(debug)<< "Hough transform [helix] " << hough_Helix.size();

      // ======================================

      // 3. Curl hough ========================
      std::vector<std::vector<TVector3>> hough_Curl;
      if(FindUsingHough(graphFoundHits, hough_Curl, FgdReconTemplate::HoughType::CURL)
        && !hough_Curl.empty()) 
      {
        size_t localBiggest(0);
        size_t ind(0);
        for(size_t j = 0; j < hough_Curl.size(); ++j)
        {
          if(localBiggest < hough_Curl[j].size())
          {
            localBiggest = hough_Curl[j].size();
            ind = j;
          }
        }

        if(localBiggest > biggestSize)
        {
          biggestSize = hough_Curl[ind].size();
          trackToAdd = &hough_Curl[ind];
          LOG(debug) << "Hough transform from graph [helix] size of track is: " << hough_Curl[ind].size();
        }
      }
      LOG(debug)<< "Hough transform [curl] " << hough_Curl.size();

      // ======================================

      if(trackToAdd!=nullptr)
      {
        foundTracks.emplace_back(*trackToAdd);
      }
      else
      {
        foundTracks.push_back(graphTracks[i]);
        LOG(debug)<< "Hough transform could not find track, adding initial graph track. ";
      }
      
    }
  }

  return rc;
}


Bool_t FgdGenFitRecon::FindUsingHoughGraphLeaves(std::vector<ReconHit>& hits
                  , std::vector<std::vector<TVector3>>& foundTracks)
{
  freconTemplate.BuildGraph(hits);
  freconTemplate.FindLeaves(hits);

  std::vector<std::vector<ReconHit*>> reconHitTracks;

  for(size_t i = 0; i < hits.size(); ++i)
  {
    if(!hits[i].fIsLeaf || hits[i].IsAlone()) continue;

    ReconHit& hit = hits[i];

    size_t biggestSize(0);
    std::vector<TVector3>* trackToAdd = nullptr;

    // 1. Line hough ========================
    std::vector<std::vector<TVector3>> hough_Line;
    if(freconTemplate.FindUsingHough(hits
                                , reconHitTracks
                                , hough_Line
                                , FgdReconTemplate::HoughType::STRAIGHT_LINE
                                , hit.fHitPos
                                , true)
        && !hough_Line.empty())
    {
      size_t localBiggest(0);
      size_t ind(0);
      for(size_t j = 0; j < hough_Line.size(); ++j)
      {
        if(localBiggest < hough_Line[j].size())
        {
          localBiggest = hough_Line[j].size();
          ind = j;
        }
      }

      if(localBiggest > biggestSize)
      {
        biggestSize = hough_Line[ind].size();
        trackToAdd = &hough_Line[ind];
        LOG(debug) << "Hough transform from graph [line] size of track is: " << hough_Line[ind].size();
      }
    }
    LOG(debug)<< "Hough transform [line] " << hough_Line.size();

    // ======================================

    // 2. Helix hough ========================
    std::vector<std::vector<TVector3>> hough_Helix;
    if(freconTemplate.FindUsingHough(hits
                                , reconHitTracks
                                , hough_Helix
                                , FgdReconTemplate::HoughType::HELIX
                                , hit.fHitPos
                                , true)
      && !hough_Helix.empty()) 
    {
      size_t localBiggest(0);
      size_t ind(0);
      for(size_t j = 0; j < hough_Helix.size(); ++j)
      {
        if(localBiggest < hough_Helix[j].size())
        {
          localBiggest = hough_Helix[j].size();
          ind = j;
        }
      }

      if(localBiggest > biggestSize)
      {
        biggestSize = hough_Helix[ind].size();
        trackToAdd = &hough_Helix[ind];
        LOG(debug) << "Hough transform from graph [helix] size of track is: " << hough_Helix[ind].size();
      }
    }
    LOG(debug)<< "Hough transform [helix] " << hough_Helix.size();

    // ======================================

    // 3. Curl hough ========================
    std::vector<std::vector<TVector3>> hough_Curl;
    if(freconTemplate.FindUsingHough(hits
                                , reconHitTracks
                                , hough_Curl
                                , FgdReconTemplate::HoughType::CURL
                                , hit.fHitPos
                                , true)
      && !hough_Curl.empty()) 
    {
      size_t localBiggest(0);
      size_t ind(0);
      for(size_t j = 0; j < hough_Curl.size(); ++j)
      {
        if(localBiggest < hough_Curl[j].size())
        {
          localBiggest = hough_Curl[j].size();
          ind = j;
        }
      }

      if(localBiggest > biggestSize)
      {
        biggestSize = hough_Curl[ind].size();
        trackToAdd = &hough_Curl[ind];
        LOG(debug) << "Hough transform from graph [helix] size of track is: " << hough_Curl[ind].size();
      }
    }
    LOG(debug)<< "Hough transform [curl] " << hough_Curl.size();

    // ======================================

    if(trackToAdd!=nullptr)
    {
      foundTracks.emplace_back(*trackToAdd);
    }
  }

  return true;
}

Bool_t FgdGenFitRecon::CalculateInitialMomentum(const std::vector<TVector3>& track,const TVector3& magField, TVector3& momentum , TVector3& momentumLoss)
{
  if(track.empty())
  {
      momentum.SetXYZ(0.,0.,0);
      momentumLoss.SetXYZ(0.,0.,0);
      return false;
  }

  Int_t segment = fParams.ParamAsInt(esbroot::geometry::superfgd::DP::FGD_TRACK_MOMENTUM_SEGMENT);
  Int_t avgInitialPoints = fParams.ParamAsInt(esbroot::geometry::superfgd::DP::FGD_INITIAL_TRACK_POINTS_MOMENTUM);
  static const Double_t inf = std::numeric_limits<Double_t>::infinity();

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
    //size_t p2_pos = p1_pos + lengthSize/2;
    std::vector<TVector3> segmentMomentums;

    while(p2_pos<i)
    {
      TVector3 segmentMom(0,0,0);
      TVector3 p2 = track[p2_pos];
      if(CalculateMomentum(p1, p2, p3, magField, segmentMom)  
          &&  segmentMom.Mag()!=inf
          && !std::isnan(momentum.Mag()))
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

  Bool_t isValid = (!std::isnan(momentum.Mag()) && momentum.Mag()!=inf && trackMomentums.size() > 0 );

  return isValid;
}

// Bool_t FgdGenFitRecon::CalculateInitialMomentum(const std::vector<TVector3>& track,const TVector3& magField, TVector3& momentum , TVector3& momentumLoss)
// {
//   momentum.SetXYZ(0.,0.,0);
//   if(track.empty())
//   {
//       return false;
//   }

//   static const Double_t inf = std::numeric_limits<Double_t>::infinity();

//   TVector3 p1 = track[0];
//   TVector3 p2 = track[track.size()/2];
//   TVector3 p3 = track[track.size()-1];

//   return CalculateMomentum(p1, p2, p3, magField, momentum)  &&  momentum.Mag()!=inf;
//}

Bool_t FgdGenFitRecon::CalculateMomentum(const TVector3& p1, const TVector3& p2, const TVector3& p3 , const TVector3& magField, TVector3& momentum)
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
Double_t FgdGenFitRecon::GetRadius(const TVector3& p1, const TVector3& p2, const TVector3& p3)
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

Int_t FgdGenFitRecon::GetPdgCode(const TVector3& momentum, const TVector3& momentumLoss , Int_t tryFit, Int_t& momCoeff)
{
  //Int_t pdgCode = 13; // Muon pdg code

  // for(size_t k=0; k<fpdgFromMomLoss.size(); ++k)
  // {
  //   PdgFromMomentumLoss& p = fpdgFromMomLoss[k];
  //   Double_t momtoLoss = momentum.Mag()/ momentumLoss.Mag();
  //   if(p.GetPdg(momtoLoss, pdgCode))
  //   {
  //     break;
  //   }
  // }

  Int_t pdgGuess = tryFit / 3;

  Int_t pdgCode = PDG_FIT_TRACK::MUON;
  switch(pdgGuess)
  {
      case 0: 
          {
            pdgCode = PDG_FIT_TRACK::MUON;
          }
        break;

      case 1:
          {
            pdgCode = PDG_FIT_TRACK::PION;
          }
        break;

      case 2:
          {
            pdgCode = PDG_FIT_TRACK::PROTON;
          }
        break;

      case 3:
          {
            pdgCode = PDG_FIT_TRACK::ELECTRON;
          }
        break;

      default:
          {

          }
        break;
  }

  momCoeff = (1 << (tryFit % 3));
  LOG(debug) << "Trying with pdg code = " << pdgCode << " , momCoeff = " << momCoeff;
  return pdgCode;
}

void FgdGenFitRecon::FitTracks(std::vector<std::vector<TVector3>>& foundTracks)
{
    fTracksArray->Delete();
    
    // init geometry and mag. field
    TVector3 magField = fgdConstructor.GetMagneticField(); // values are in kGauss
    genfit::FieldManager::getInstance()->init(new genfit::ConstField(magField.X(),magField.Y(), magField.Z())); 
    genfit::MaterialEffects::getInstance()->init(new genfit::TGeoMaterialInterface());
    genfit::MaterialEffects::getInstance()->setDebugLvl(fDebuglvl_genfit);

    // init fitter
    std::shared_ptr<genfit::AbsKalmanFitter> fitter = make_shared<genfit::KalmanFitterRefTrack>();
    // std::shared_ptr<genfit::AbsKalmanFitter> fitter = make_shared<genfit::KalmanFitter>();
    fitter->setMinIterations(fminGenFitInterations);
    fitter->setMaxIterations(fmaxGenFitIterations);
    fitter->setDebugLvl(fDebuglvl_genfit);

    std::vector<genfit::Track*> genTracks;
    int detId(1); // Detector id, it is the same, we only have one detector

    // If the track could not be fitted because of Bette-bloch calculations
    // try to double the momentum
    //Bool_t isMomentumLow(false);
    // Int_t momCoef = 1;
    // Int_t limitCoeff = 4;
    // Bool_t tryFit(false);
    // Int_t pdgGuessCounter(0);
    // Int_t limitPdgGuess(4);

    Bool_t tryFit(false);
    Int_t tryCounter(1);
    Int_t limitTryFit = 1; // 12 = 3 times increse momentum by 2, 4 pdg codes to try


    for(size_t i = 0; i <  foundTracks.size() ; ++i)
    {
      // if(momCoef>=limitCoeff)
      // {
      //   isMomentumLow = false;
      //   momCoef = 1;
      // }

      // if(isMomentumLow)
      // {
      //   momCoef = momCoef * 2;
      //   --i;
      //   isMomentumLow = false;
      //   LOG(debug) << "Momentum too low, doubling momentum estimate";
      // }

      if(tryFit && tryCounter < limitTryFit)
      {
        tryCounter++;
        tryFit = false;
        --i;
      }

      std::vector<TVector3>& hitsOnTrack = foundTracks[i];
      LOG(debug) << "Track " << i;
      for(size_t j =0; j< hitsOnTrack.size(); ++j)
      {
          TVector3& vec = hitsOnTrack[j];
          LOG(debug) << "X " << vec.X() + f_total_X/2 << " Y " << vec.Y() + f_total_Y/2 << " Z " << vec.Z() + f_total_Z/2;
      }

      // Set lower limit on track size
      if(hitsOnTrack.size()<fminHits)
      {
        LOG(debug) << "Track " << i << " below limit, continue with next track (" << hitsOnTrack.size() << " < " << fminHits << ")";
        continue;
      }
      
    
      TVector3 posM(hitsOnTrack[0].X(),hitsOnTrack[0].Y(),hitsOnTrack[0].Z());
      TVector3 momM(0,0,0);
      TVector3 momLoss(0,0,0);

      if(!CalculateInitialMomentum(hitsOnTrack, magField, momM, momLoss))
      {
        LOG(debug) << "Track " << i << " unable to extract momentum. Continue with next track";
        continue;
      }

      Int_t momCoeff(1);
      Int_t pdg = GetPdgCode(momM, momLoss, tryCounter, momCoeff);
      momM = momM * momCoeff;
      

      // approximate covariance
      const double resolution = 1;// Default in example is 0.1;
      TMatrixDSym hitCov(3);
      hitCov(0,0) = resolution*resolution;
      hitCov(1,1) = resolution*resolution;
      hitCov(2,2) = resolution*resolution;

      TMatrixDSym covM(6);
      for (int ci = 0; ci < 3; ++ci)
          covM(ci,ci) = resolution*resolution;
      for (int ci = 3; ci < 6; ++ci)
          covM(ci,ci) = covM(ci,ci) = pow(  ((resolution / hitsOnTrack.size()) / sqrt(3)), 2); 

      // trackrep
      genfit::AbsTrackRep* rep = new genfit::RKTrackRep(pdg);

      // smeared start state
      genfit::MeasuredStateOnPlane stateSmeared(rep);
      stateSmeared.setPosMomCov(posM, momM, covM);

      // create track
      TVectorD seedState(6);
      TMatrixDSym seedCov(6);
      stateSmeared.get6DStateCov(seedState, seedCov);
  
      genfit::Track* toFitTrack = new genfit::Track(rep, seedState, seedCov);

      size_t bhlimit = hitsOnTrack.size();
      //bhlimit = bhlimit * (1./tryCounter);

      LOG(debug) << "******************************************* ";
      LOG(debug) << "******    Track "<< i << "  ************************";
      LOG(debug) << "******************************************* ";
      LOG(debug) << " \tPdg code " << pdg;
      LOG(debug) << " \tHits in track "<< bhlimit;
      LOG(debug) << " \tTrack Momentum [" << momM.Mag() << "]" << "(" << momM.X() << "," << momM.Y() << "," << momM.Z() << ")";
      LOG(debug) << " \tTrack Momentum Loss [" << momLoss.Mag() << "]" << "(" << momLoss.X() << "," << momLoss.Y() << "," << momLoss.Z() << ")";
      LOG(debug) << " \tMomentum / Momentum Loss [" << momM.Mag()/momLoss.Mag() << "]";
      
      
      for(Int_t bh = 0; bh < bhlimit; ++bh)
      {
        TVectorD hitPos(3);
        hitPos(0) = hitsOnTrack[bh].X();
        hitPos(1) = hitsOnTrack[bh].Y();
        hitPos(2) = hitsOnTrack[bh].Z();

        genfit::AbsMeasurement* measurement = new genfit::SpacepointMeasurement(hitPos, hitCov, detId, bh, nullptr);
        toFitTrack->insertMeasurement(measurement, bh);

        //std::vector<genfit::AbsMeasurement*> measurements{measurement};
        //toFitTrack->insertPoint(new genfit::TrackPoint(measurements, toFitTrack));
      }

      try
      {
        //check
        toFitTrack->checkConsistency();

        // do the fit
        fitter->processTrack(toFitTrack, true);

        //check
        toFitTrack->checkConsistency();

        PrintFitTrack(*toFitTrack);

        LOG(debug) <<"******************************************* ";
        genfit::FitStatus* fiStatuStatus = toFitTrack->getFitStatus();

        if(fiStatuStatus->isFitted())
        {
          genTracks.push_back(toFitTrack);
        }
        
        //isMomentumLow = fiStatuStatus->isFitted() && !fiStatuStatus->isFitConverged();
        tryFit = fiStatuStatus->isFitted() && !fiStatuStatus->isFitConverged();
        
      }
      catch(genfit::Exception& e)
      {
          LOG(error) <<"Exception, when tryng to fit track";
          LOG(error) << "e.what() " << e.what();
          LOG(error) << "e.getExcString() " << e.getExcString();

          Bool_t isFittedError = (e.getExcString().find("KalmanFitterInfo::getFittedState") != std::string::npos)
                                  || (e.getExcString().find("Track::getFittedState") != std::string::npos);
          
          //isMomentumLow = (e.getExcString().find("KalmanFitterInfo::getFittedState") != std::string::npos);
          tryFit = isFittedError && (tryCounter <= limitTryFit);
      }
    }
  
    if(isGenFitVisualization)
    {
      fdisplay->addEvent(genTracks);
    }
}

void FgdGenFitRecon::DefineMaterials() 
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

void FgdGenFitRecon::PrintFitTrack(genfit::Track& fitTrack)
{
  const genfit::MeasuredStateOnPlane& me = fitTrack.getFittedState();
  //LOG(debug)<< "Momentum  " << (me.getMom()).Mag();
  //LOG(debug)<< " X  " << (me.getMom()).X()<< " Y " << (me.getMom()).Y()<< " Z  " << (me.getMom()).Z();
  LOG(debug)<< "\tFitted Momentum  [" << (me.getMom()).Mag() <<"]" << "(" << (me.getMom()).X() << "," << (me.getMom()).Y() << "," << (me.getMom()).Z() << ")";
  genfit::FitStatus* fiStatuStatus = fitTrack.getFitStatus();
  fiStatuStatus->Print();

  LOG(debug)<< "fiStatuStatus->isFitted()  " << fiStatuStatus->isFitted();
  LOG(debug)<< "fiStatuStatus->isFitConverged()  " << fiStatuStatus->isFitConverged();
  LOG(debug)<< "fiStatuStatus->isFitConvergedFully()  " << fiStatuStatus->isFitConvergedFully();
  LOG(debug)<< "fiStatuStatus->isFitConvergedPartially()  " << fiStatuStatus->isFitConvergedPartially();
  LOG(debug)<< "fitTrack.getNumPoints() " << fitTrack.getNumPoints();
}

Long_t FgdGenFitRecon::ArrInd(int x, int y, int z)
{
  return (x*f_bin_Y*f_bin_Z + y*f_bin_Z+z);
}

// -------------------------------------------------------------------------

}// namespace superfgd
}// namespace reconstruction
}// namespace esbroot
