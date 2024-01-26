#ifndef ESBROOT_ESBDRECONSTRUCTION_FGD_GENFIT_H
#define ESBROOT_ESBDRECONSTRUCTION_FGD_GENFIT_H

// EsbRoot headers
#include "EsbData/EsbSuperFGD/FgdHit.h"
#include "EsbGeometry/EsbSuperFGD/EsbFgdDetectorParameters.h"
#include "EsbGeometry/EsbSuperFGD/EsbSuperFGDDetectorConstruction.h"
#include "EsbReconstruction/EsbSuperFGD/FgdReconHit.h"
#include "EsbReconstruction/EsbSuperFGD/PdgFromMomentumLoss.h"
#include "EsbReconstruction/EsbSuperFGD/FgdReconTemplate.h"

// FairRoot headers
#include <FairTask.h>

// ROOT headers
#include <include/EventDisplay.h>

// Pathfinder headers
#include "basicHit.h"
#include "TrackFinderTrack.h"

namespace esbroot {
namespace reconstruction {
namespace superfgd {

class FgdGenFitRecon : public FairTask
{

 public:

  enum TrackFinder{
    HOUGH_PATHFINDER_LINE,
    HOUGH_PATHFINDER_HELIX,
    HOUGH_PATHFINDER_CURL,
    GRAPH,
    GRAPH_HOUGH_PATHFINDER,
    HOUGH_GRAPH_LEAVES
  };

  enum PDG_FIT_TRACK
  {
    MUON = 13,
    PION = 211,
    PROTON = 2212,
    ELECTRON = 11
  };

  /** Default constructor **/  
  FgdGenFitRecon();

  /** Constructor with argument
   *@param name       Name of task
   *@param geoConfigFile  - Configuration file detector
   *@param graphConfig  - Configuration file for graph algorithm
   *@param mediaFile  - Configuration file for the used mediums
   *@param verbose  - Verbosity level
   *@param debugLlv - debug level for genfit
   *@param visualize -  to visualize the event using genfit::EventDisplay
   *@param visOption -  option to be passed to genfit::EventDisplay
  **/  
  FgdGenFitRecon(const char* name
              , const char* geoConfigFile
              , const char* graphConfig
              , const char* mediaFile
              , Int_t verbose = 1
              , double debugLlv = 0
              , bool visualize = false
              , std::string visOption ="D");

  /** Destructor **/
  virtual ~FgdGenFitRecon();

  void SetMinInterations(Int_t minIterations) {fminGenFitInterations = minIterations;}
  void SetMaxInterations(Int_t maxIterations) {fmaxGenFitIterations = maxIterations;}
  void SetMinHits(Int_t minHits) {fminHits = minHits;}
  void SetUseTracker(FgdGenFitRecon::TrackFinder finder){ffinder=finder;}
  void AddPdgMomLoss(Int_t pdg, Double_t momLoss, Double_t allowDiff){ fpdgFromMomLoss.emplace_back(pdg, momLoss, allowDiff);}

  /** Virtual method Init **/
  virtual InitStatus Init() override;
  virtual void OutputFileInit(FairRootManager* manager); // Create virtual method for output file creation
  virtual void FinishEvent() override;
  virtual void FinishTask() override;


  /** Virtual method Exec **/
  virtual void Exec(Option_t* opt) override;

protected:

  enum FindTrackType{
    STRAIGHT_LINE,
    HELIX,
    CURL
  };

  /** Get all hits **/
  Bool_t GetHits(std::vector<ReconHit>& allHits);

  /** Extrack tracks from the hit using Hough Transform **/
  Bool_t FindUsingHough(std::vector<ReconHit>& hits
                  , std::vector<std::vector<TVector3>>& foundTracks
                  , FgdReconTemplate::HoughType houghType);

  /** Extrack tracks using graph traversal and track gradient  **/
  Bool_t FindUsingGraph(std::vector<ReconHit>& hits
                  , std::vector<std::vector<TVector3>>& foundTracks
                  , std::vector<std::vector<ReconHit*>>& reconTracks);

  /** Extrack tracks using graph traversal and track gradient and then apply Hough transform for each track **/
  Bool_t FindUsingGraphHough(std::vector<ReconHit>& hits
                  , std::vector<std::vector<TVector3>>& foundTracks);

  /** Extrack tracks using graph leaves as vertexes for each track **/
  Bool_t FindUsingHoughGraphLeaves(std::vector<ReconHit>& hits
                  , std::vector<std::vector<TVector3>>& foundTracks);
  
  Bool_t CalculateInitialMomentum(const std::vector<TVector3>& track,const TVector3& magField, TVector3& momentum, TVector3& momentumLoss);
  Bool_t CalculateMomentum(const TVector3& p1, const TVector3& p2, const TVector3& p3 , const TVector3& magField, TVector3& momentum);
  Double_t GetRadius(const TVector3& p1, const TVector3& p2, const TVector3& p3);
  Int_t GetPdgCode(const TVector3& momentum, const TVector3& momentumLoss, Int_t tryFit, Int_t& momCoeff);;

  /** Fit the found tracks using genfit **/
  void FitTracks(std::vector<std::vector<TVector3>>& foundTracks);

  /** Define materials used in the reconstruction phase **/
  void DefineMaterials();

  /** Print information for fitted grack **/
  void PrintFitTrack(genfit::Track& track);

  Long_t ArrInd(int i, int j, int k);

  /** Class to hold the Detector parameters read from external file **/
  esbroot::geometry::superfgd::FgdDetectorParameters fParams;

  /** Class containing the TGeometry for reconstruction of the tracks **/
  esbroot::geometry::superfgd::SuperFGDDetectorConstruction    fgdConstructor;	   //! SuperFgd Detector Constructor

  /** Detector geometry pointer**/
  TGeoVolume* fsuperFgdVol;

  /** Input array of FgdDetectorPoint(s)**/
  TClonesArray* fHitArray;     //! 

  /** Output array with genfit::Track(s) **/
  TClonesArray* fTracksArray;        //!

   /** Detector dimentions **/
  Double_t flunit;
  double f_step_X;
  double f_step_Y;
  double f_step_Z;
  int f_bin_X;
  int f_bin_Y;
  int f_bin_Z;
  double f_total_X;
  double f_total_Y;
  double f_total_Z;

  /** Start position and momentum **/
  //  0 - no additional info
  //  1 - debug info
  //  2- detail info
  double fDebuglvl_genfit;

  FgdGenFitRecon::TrackFinder ffinder;

  FgdReconTemplate freconTemplate;//!<!
 
  /** Path to the used media.geo file - containing definitions of materials **/
  std::string fmediaFile;

  Int_t fminGenFitInterations;
  Int_t fmaxGenFitIterations;
  Int_t fminHits;

  /** Are materials already defined **/
  bool isDefinedMaterials;

  /** local Members for genfit visualization  **/
  genfit::EventDisplay* fdisplay;//!<!
  bool isGenFitVisualization;//!<!
  std::string fGenFitVisOption;//!<!

  std::vector<PdgFromMomentumLoss> fpdgFromMomLoss;//!<!

  	   
  ClassDef(FgdGenFitRecon, 2);

};

} //superfgd
} //reconstruction
} //esbroot

#endif // ESBROOT_ESBDRECONSTRUCTION_FGD_GENFIT_H
