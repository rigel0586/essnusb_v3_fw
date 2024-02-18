#ifndef ESBROOT_ESBDRECONSTRUCTION_FGD_TMVA_DATA_STATS_H
#define ESBROOT_ESBDRECONSTRUCTION_FGD_TMVA_DATA_STATS_H

// EsbRoot headers
#include "reconstruction/SuperFGD/FgdMCGenFitRecon.hpp"
#include "reconstruction/SuperFGD/FgdTMVAEventRecord.hpp"
#include "reconstruction/SuperFGD/FgdReconTemplate.hpp"
#include <TH1F.h>

#define PHOTON_SPECTRUM_SIZE 50
#define PHOTON_SPECTRUM_MAX 300

#define EVENT_TIME_SPECTRUM_SIZE 100
#define EVENT_TIME_SPECTRUM_MAX 2

namespace esbroot {
namespace reconstruction {
namespace superfgd {

class FgdTMVAData : public FgdMCGenFitRecon
{

 public:

  /** Default constructor **/  
  FgdTMVAData();

  /** Constructor with argument
   *@param name       Name of task
   *@param geoConfigFile  - Configuration file detector
   *@param graphConfig  - Configuration file for graph algorithm
   *@param eventData  - events data file (generated from fgd generator)
   *@param outputRootFile - full path to the output root file
   *@param verbose  - Verbosity level
   *@param debugLlv - debug level for genfit
  **/  
  FgdTMVAData(const char* name
              , const char* geoConfigFile
              , const char* graphConfig
              , const char* eventData
              , const char* outputRootFile
              , Int_t verbose = 1
              , double debugLlv = 0);

  /** Destructor **/
  virtual ~FgdTMVAData();

  /** Virtual method Init **/
  virtual bool Init() override;
  virtual void afterEvent() override;
  virtual void afterRun() override;


  /** Virtual method Exec **/
  virtual bool Exec(int eventId, TClonesArray* data) override;

protected:

  Bool_t ProcessStats(std::vector<std::vector<ReconHit>>& foundTracks, std::vector<ReconHit>& allhits);
  Double_t CalculatePhotoEdep(ReconHit& hit);
  
  Bool_t isParticleAllowed(Int_t pdg);
  void clearSpectrum();
  void copySpectrum(size_t ind);

  void clearTime();
  void copyTimeSpectrum(size_t ind);

  void printNotUsed();

  std::string feventData;//!<!
  std::string foutputRootFile;//!<!

  std::vector<FgdTMVAEventRecord> feventRecords;//!<!
  std::shared_ptr<TDatabasePDG> fpdgDB;//!<!

  std::map<Int_t, std::vector<TVector3>> fhitCoordinates;//!<!
  std::map<Int_t, std::vector<TVector3>> fhitPhotons;//!<!

  std::vector<std::vector<Float_t>> ftrackLenghts;//!<!
  std::vector<std::vector<Float_t>> ftrackPhotos;//!<!
  std::vector<std::vector<Float_t>> ftrackEntryTimes;//!<!

  std::vector<std::vector<Float_t>> fphotonSpectrum;//!<!
  std::vector<std::vector<Float_t>> fTimeSpectrum;//!<!

  int feventNum;//!<!
  Float_t fmagField_X;
  Float_t fmagField_Y;
  Float_t fmagField_Z;

  Float_t fMaxtrack;
  Float_t fMaxTrph;

  Float_t fMaxTotph;
  Float_t fMaxCubes;
  Float_t fMaxTotEdep;
  Float_t fMaxTotPe;
  Float_t fMaxnuE;
  Float_t fMaxTrueEdep;

  Float_t fPhSpecter[PHOTON_SPECTRUM_SIZE];
  Bool_t fSpecConst[PHOTON_SPECTRUM_SIZE];
  TH1F* f_hist_spectrum;

  Float_t fTimeSpec[EVENT_TIME_SPECTRUM_SIZE];
  Bool_t fTimeSpecConst[EVENT_TIME_SPECTRUM_SIZE];
  TH1F* f_hist_time;

  FgdReconTemplate freconTemplate;//!<!
  	   
  ClassDef(FgdTMVAData, 2);

};

} //superfgd
} //reconstruction
} //esbroot

#endif // ESBROOT_ESBDRECONSTRUCTION_FGD_TMVA_DATA_STATS_H
