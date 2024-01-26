#ifndef ESBROOT_DATA_SUPERFGD_FGD_DETECTOR_PARAMETERS_H
#define ESBROOT_DATA_SUPERFGD_FGD_DETECTOR_PARAMETERS_H 1

#include "CLHEP/Units/SystemOfUnits.h"
#include "CLHEP/Units/PhysicalConstants.h"

#include "TGeoManager.h"
#include <TGeoBBox.h>
#include <TGeoVolume.h>

#include "TObject.h"

using namespace std;
        
namespace esbroot {

namespace geometry {

namespace superfgd {


namespace DP
{
    static const string  length_X = "length_X";
    static const string  length_Y = "length_Y";
    static const string  length_Z = "length_Z";

    static const string  number_cubes_X = "number_cubes_X";
    static const string  number_cubes_Y = "number_cubes_Y";
    static const string  number_cubes_Z = "number_cubes_Z";

    static const string  useFiber = "useFiber";
    static const string  useSimpleCube = "useSimpleCube";
    
    static const string  visdetail = "visdetail";

    static const string  magField_X = "magField_X";
    static const string  magField_Y = "magField_Y";
    static const string  magField_Z = "magField_Z";

    static const char SEPERATOR='=';

    static const string FGD_BRANCH = "EsbFgdDetectorPoint";
    static const string FGD_DETECTOR_NAME = "EsbFgdDetector";
    static const string FGD_HIT = "FgdHit";
    static const string FGD_BRANCH_FIT = "FgdGenFitBranch";
    static const string FGD_FIT_TRACK = "FgdGenFitTrack";

    static const string FGD_MC_LEPTON_RECONSTRUCTION_TTREE = "FgdMCLeptonStatsReconstructionData";
    static const string FGD_MC_LEPTON_RECONSTRUCTION_BRANCH = "FgdMCLeptonStatsBranch";
    static const string FGD_MC_LEPTON_RECONSTRUCTION_ROOT_FILE = "FgdMCLeptonStatsRootFile";

    static const string FGD_MC_EXIT_PARTICLES_RECONSTRUCTION_TTREE = "FgdMCExitParticlesReconstructionData";
    static const string FGD_MC_EXIT_PARTICLES_RECONSTRUCTION_ROOT_FILE = "FgdMCExitParticlesRootFile";
    static const string FGD_MC_EXIT_PARTICLES_RECONSTRUCTION_BRANCH = "FgdMCExitParticlesBranch";

    static const string FGD_MC_EXIT_PARTICLES_RECONSTRUCTION_TTREE_ELECTRON = "FgdMCExitParticlesReconstructionData_Electron";
    static const string FGD_MC_EXIT_PARTICLES_RECONSTRUCTION_BRANCH_ELECTRON = "FgdMCExitParticlesBranchElectron";

    static const string FGD_MC_EXIT_PARTICLES_RECONSTRUCTION_TTREE_MUON = "FgdMCExitParticlesReconstructionData_Muon";
    static const string FGD_MC_EXIT_PARTICLES_RECONSTRUCTION_BRANCH_MUON = "FgdMCExitParticlesBranchMuon";

    static const string FGD_TMVA_DATA_TTREE = "FgdTMVAData";
    static const string FGD_TOTAL_PHOTONS_TTREE = "FgdTotalPhotonsTree";
    static const string FGD_TRACK_PROJECTION_TTREE = "FgdTrackProjectionTree";
    static const string FGD_LONGEST_TRACK_PROJECTION_TTREE = "FgdLongestProjectionTree";
    static const string FGD_DEPOSITED_ENERGY_CUBE_TTREE = "FgdDepositedEnergyTree";
    static const string FGD_CUBE_INFO_TTREE = "FgdCubesEventInfoTree";
    static const string FGD_TMVA_DATA_BRANCH = "FgdTMVADataBranch";
    static const string FGD_TMVA_HIT_ARRAY_BRANCH = "FgdTMVAHitArrayBranch";
    static const string FGD_TMVA_PHOTO_ARRAY_BRANCH = "FgdTMVAPhotoArrayBranch";
    static const string FGD_TMVA_DATA_ROOT_FILE = "FgdTMVADataRootFile";

    static const string FGD_MIN_TRACK_LENGTH = "min_track_lenght";

    static const string FGD_GRAD_DIST = "gradDist";
    static const string FGD_GRAD_INTERVAL_DIST = "gradIntervalDist";
    static const string FGD_GRAD_ALLOWABLE_DIFF = "gradDiff";

    static const string FGD_SMOOTH_GRAPH_DEPTH = "smoothdeepth";
    static const string FGD_SMOOTH_GRAPH_ERR_LIMIT = "smootheErrLimit";

    static const string FGD_TRACK_MOMENTUM_SEGMENT = "momTrackSegment";
    static const string FGD_INITIAL_TRACK_POINTS_MOMENTUM = "avgTrackMomentum";

    static const string FGD_ERR_PHOTO_LIMIT = "errPhotoLimit";

    static const string PF_USE_VERTEX = "use_vertex";
    static const string PF_VERTEXX = "vertexX";
    static const string PF_VERTEXY = "vertexY";
    static const string PF_MAXDISTXY = "maxdistxy";
    static const string PF_MAXDISTSZ = "maxdistsz";
    static const string PF_MAXDISTXYFIT = "maxdistxyfit";
    static const string PF_MAXDISTSZFIT = "maxdistszfit";
    static const string PF_MINHITNUMBER = "minhitnumber";
    static const string PF_XYTHETABINS = "xythetabins";

    static const string PF_XYD0BINS = "xyd0bins";
    static const string PF_XYOMEGABINS = "xyomegabins";
    static const string PF_SZTHETABINS = "szthetabins";
    static const string PF_SZD0BINS = "szd0bins";
    static const string PF_SEACHINTHENEIGHBORHOOD = "searchneighborhood";
}

class FgdDetectorParameters : public TObject
{
public:
    /** Default constructor **/
    FgdDetectorParameters();

    /** Destructor **/
    ~FgdDetectorParameters();

    /** Read the detector parameters from file
     *@param fullfilePathName - full path of the file
     **/
    void LoadPartParams(string fullfilePathName);

    /** Return the dimentions used for the detector **/
    static Double_t GetLenghtUnit() {return 1.0; /* Default length unit in Root is in 'cm' */}  

    /** Return the existing map parameter as std::string  **/
    string ParamAsString(const string& paramName) const;
    /** Return the existing map parameter as Int_t  **/
    Int_t       ParamAsInt( const string& paramName) const;
    /** Return the existing map parameter as bool  **/
    Bool_t        ParamAsBool(const string& paramName) const;
    /** Return the existing map parameter as Double_t  **/
    Double_t    ParamAsDouble( const string& paramName) const;
    /** Check if parameters exists in the map **/
    Bool_t    ExistsParam(const string& paramName) const;

    /** Return the existing parameters **/
    map<string, string>& GetParamsMap() {return fparamsMap;}
    /** Set the parameter map **/
    void  SetParamsMap(const map<string, string>& newMap){fparamsMap = newMap;}

    FgdDetectorParameters& operator=(FgdDetectorParameters& newDp);

private:

    mutable map<string, string> fparamsMap;

    ClassDef(FgdDetectorParameters,2)
};

}   // superfgd
}   // geometry
}   // esbroot

#endif
