#ifndef ESBROOT_ESBDIGITIZER_FGD_MPPC_DISPLAY_H
#define ESBROOT_ESBDIGITIZER_FGD_MPPC_DISPLAY_H

#include <TClonesArray.h>
#include "TRandom3.h"
#include <TCanvas.h>

#include "geometry/SuperFGD/EsbSuperFGD/FgdDetectorParameters.h"
#include "data/SuperFGD/FgdHit.hpp"
#include "core/task/ITask.hpp"

namespace esbroot {
namespace digitizer {
namespace superfgd {


class FgdMppcDisplay : public core::task::ITask 
{

 public:

  /** Default constructor **/  
  FgdMppcDisplay();

  /** Constructor with argument
   *@param name       Name of task
   *@param geoConfigFile  Configuration file detector
   *@param x - X coordinate of the detector
   *@param y - Y coordinate of the detector
   *@param z - Z coordinate of the detector
   *@param verbose   Verbosity level
  **/  
  FgdMppcDisplay(const char* name
              ,const char* geoConfigFile
              ,const char* outPath
              ,double x, double y, double z
              ,Int_t verbose = 1);

  /** Destructor **/
  ~FgdMppcDisplay();


  /** Virtual method Init **/
  virtual bool Init() override;

  virtual void afterEvent() override;
  virtual void afterRun() override;


  /** Virtual method Exec **/
  virtual bool Exec(int eventId, TClonesArray* data) override;

private:

  /** Class to hold the Detector parameters read from external file **/
  esbroot::geometry::superfgd::FgdDetectorParameters fParams;

  std::string fOutPath{""};

  /** Coordinates of the detector **/
  double fX;
  double fY;
  double fZ;

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

  std::shared_ptr<double*> xyz;// array pointer to total energy deposited in cubes

  TH2F* f_xy_hist;
  TH2F* f_yz_hist;
  TH2F* f_xz_hist;
  TH1F* f_ph_hist;
  TH1F* f_ph_hist_total;
  TH1F* f_time_hist;
  double f_photo_ave;
  int f_photo_count;

  /** Input array of FgdHit(s)**/
  TClonesArray* fHitArray;        //!

  // Export hist to gif
  TCanvas* fcanvas;
  int fevNum;
  void WriteCanvas(string hist);
  	   
  ClassDef(FgdMppcDisplay, 2);

};

} //superfgd
} //digitizer
} //esbroot

#endif // ESBROOT_ESBDIGITIZER_FGD_MPPC_DISPLAY_H
