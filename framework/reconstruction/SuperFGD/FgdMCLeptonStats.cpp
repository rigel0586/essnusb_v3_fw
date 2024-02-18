#include "reconstruction/SuperFGD/FgdMCLeptonStats.hpp"
ClassImp(esbroot::reconstruction::superfgd::FgdMCLeptonStats)

#include "reconstruction/SuperFGD/FgdReconTemplate.hpp"
#include "reconstruction/SuperFGD/FgdCalorimetric.hpp"
#include "data/SuperFGD/FgdDetectorPoint.hpp"
#include "digitizer/SuperFGD/FgdDigitizer.hpp"


#include <fairlogger/Logger.h>

// Root headers
#include <TClonesArray.h>
#include <TEveManager.h>
#include <TGeoElement.h>
#include <TGeoManager.h>
#include <TH1F.h>
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

#define MIN_CUBES_LENGTH_FOR_TRACK 3 // Number of cubes

// -----   Default constructor   -------------------------------------------
FgdMCLeptonStats::FgdMCLeptonStats() : FgdMCGenFitRecon(), feventNum(0)
{
}
// -------------------------------------------------------------------------

// -----   Constructor   -------------------------------------------
FgdMCLeptonStats::FgdMCLeptonStats(const char* name
                          , const char* geoConfigFile
                          , const char* eventData
                          , const char* outputRootFile
                          , const char* exitParticlesFile
                          , Int_t verbose
                          , double debugLlv) :
  FgdMCGenFitRecon(name, geoConfigFile, eventData, verbose, 
                    debugLlv, false /* no visualization */, "D")
    , feventData(eventData), foutputRootFile(outputRootFile)
    , fexitParticlesFile(exitParticlesFile)
    , foutputFile(exitParticlesFile, std::ios::trunc)
    , feventNum(0) 
    , ffitMuonMom(false)
{ 
    fpdgDB = make_shared<TDatabasePDG>();

    if(foutputFile.is_open())
    {
        foutputFile << "\tEach row represents an event " << endl;
        foutputFile << "\tThe event data in the row is in the following order: " << endl;
        foutputFile << "\t\tevent_number" << endl;
        foutputFile << "\t\tnu_Pdg" << endl;
        foutputFile << "\t\tnu_Energy" << endl;
        foutputFile << "\t\tIsWeakCC (1 - yes, 0 no)" << endl;
        foutputFile << "\t\tisWeakNC (1 - yes, 0 no)" << endl;
        foutputFile << "\t\tisQuasiEalsticScattering (1 - yes, 0 no)" << endl;
        foutputFile << "\tAfter this, the exit particles are listed by having the following    " << endl;
        foutputFile << "\t\tparticle_pdg" << endl;
        foutputFile << "\t\tmomentum_X" << endl;
        foutputFile << "\t\tmomentum_Y" << endl;
        foutputFile << "\t\tmomentum_Z" << endl;
        foutputFile << "\t\tposition_X" << endl;
        foutputFile << "\t\tposition_Y" << endl;
        foutputFile << "\t\tposition_Z" << endl;
        foutputFile << "\tPosition is relative to the detector center " << endl;
        foutputFile << "================================================ " << endl;
    }
}
// -------------------------------------------------------------------------



// -----   Destructor   ----------------------------------------------------
FgdMCLeptonStats::~FgdMCLeptonStats() 
{
    if(fTracksArray) 
    {
        fTracksArray->Delete();
        delete fTracksArray;
    }
    foutputFile.close();
}
// -------------------------------------------------------------------------



// -----   Public method Init   --------------------------------------------
bool FgdMCLeptonStats::Init() 
{   
    FgdMCGenFitRecon::Init();

    std::ifstream eventFileStream;
    try
    {        
        eventFileStream.open(feventData.c_str(), std::ios::in);

        if(eventFileStream.is_open())
        {
            std::string line;
            while(std::getline(eventFileStream,line))
            {
                feventRecords.emplace_back(FgdMCEventRecord(line));
            }
        }
    }
    catch(const std::exception& e)
    {
        LOG(fatal) << e.what();
    }

    if(eventFileStream.is_open())
    {
        eventFileStream.close();
    }

    // for(size_t i =0 ; i < feventRecords.size(); i++)
    // {
    //     LOG(info) << " ===========  event " << i <<  " =========== ";
    //     LOG(info) << feventRecords[i].GetNuPdg() 
    //                 << " " << feventRecords[i].GetNuE()
    //                 << " " << feventRecords[i].IsWeakCC()
    //                 << " " << feventRecords[i].IsWeakNC()
    //                 << " " << feventRecords[i].IsQuasiElastic();
        
    //     auto par = feventRecords[i].GetPrimaryParticles();
    //     for(size_t j = 0; j < par.size(); ++j)
    //     {
    //         LOG(info) << par[j].first << " " << par[j].second.X() << " " << par[j].second.Y()<< " " << par[j].second.Z(); 
    //     }
    //     LOG(info) << " ====================== ";
    // }
    

    return true;
}

// -------------------------------------------------------------------------



// -----   Public methods   --------------------------------------------

bool FgdMCLeptonStats::Exec(int eventId, TClonesArray* data)
{  
  try
  {
    fHitArray = data;
    std::vector<ReconHit> allhits;
    std::vector<std::vector<ReconHit>> foundTracks;

    bool rc = GetHits(allhits);

    if(rc)
    { 
      LOG(debug) <<" Hits to retrieve stats from " << allhits.size();
      SplitTrack(allhits, foundTracks);
    }

    if(rc)
    {
      LOG(debug) <<" Found tracks to process " << foundTracks.size();
      ProcessStats(foundTracks);
    }
    else
    {
      LOG(error) << " Could not find clean hits or tracks! ";
      ++feventNum;
    }
  }
  catch(genfit::Exception& e)
  {
      LOG(fatal) << "Exception, when tryng to fit track";
      LOG(fatal) << e.what();
  }

  return true;
}

Bool_t FgdMCLeptonStats::ProcessStats(std::vector<std::vector<ReconHit>>& foundTracks)
{
    if(feventNum >= feventRecords.size())
    {
        LOG(fatal) << "EventData reconrds are less than the passed events!";
        throw "EventData reconrds are less than the passed events!";
    }

    FgdMCEventRecord& mcEventRecord = feventRecords[feventNum];
    mcEventRecord.ReadEventData();


    for(size_t i = 0; i <  foundTracks.size() ; ++i)
    {
        std::vector<ReconHit>& hitsOnTrack = foundTracks[i];
        // Sort by time, the 1st hit in time is the start of the track
        std::sort(hitsOnTrack.begin(), hitsOnTrack.end(), [](ReconHit& bh1, ReconHit& bh2){return bh1.ftime<bh2.ftime;});
    }

    // 1. Determine the muon track length
    if(mcEventRecord.IsPrimaryLeptonMuon())
    {
        for(size_t i = 0; i <  foundTracks.size() ; ++i)
        {
            std::vector<ReconHit>& hitsOnTrack = foundTracks[i];
            if(hitsOnTrack.empty()) continue;
            if(hitsOnTrack[0].fpdg == genie::kPdgMuon || hitsOnTrack[0].fpdg == genie::kPdgAntiMuon)
            {
                Double_t sumTrackLenght = 0;
                for(size_t j = 0; j < hitsOnTrack.size(); ++j)
                {   
                    ReconHit& hit = hitsOnTrack[j];
                    sumTrackLenght += hit.ftrackLength;
                }
                mcEventRecord.SetMuonTrackLength(sumTrackLenght);
                Double_t trOrg = hitsOnTrack[hitsOnTrack.size()-1].ftrackLengthOrigin;
                mcEventRecord.SetMuonTrackLengthOrigin(trOrg);
                break;
            }
        }
    }

    // 2. Determine if the muon is exiting the detector
    if(mcEventRecord.IsPrimaryLeptonMuon())
    {
        for(size_t i = 0; i <  foundTracks.size() ; ++i)
        {
            std::vector<ReconHit>& hitsOnTrack = foundTracks[i];
            if(hitsOnTrack.empty()) continue;
            if(hitsOnTrack[0].fpdg == genie::kPdgMuon || hitsOnTrack[0].fpdg == genie::kPdgAntiMuon)
            {
                ReconHit& lastHit = hitsOnTrack[hitsOnTrack.size() -1 ];
                // LOG(info) << "Muon is exiting " << lastHit.fmppcLoc.Z() << " f_bin_Z " << f_bin_Z << " lastHit.fmomExit.Mag() " << lastHit.fmomExit.Mag();
                if(IsHitExiting(lastHit) && lastHit.fmomExit.Mag()!=0)
                {
                    LOG(debug2) << "Muon is exiting";
                    mcEventRecord.SetMuonExiting(true);
                    mcEventRecord.SetMuonExitMom(lastHit.fmomExit);
                }
                break;
            }
        }
    }


    // 3. Sum the total energy deposited by hadrons if is a muon event
    if(mcEventRecord.IsPrimaryLeptonMuon())
    {
        Double_t totalEdepInChargedHadrons = 0;
        for(size_t i = 0; i <  foundTracks.size() ; ++i)
        {
            std::vector<ReconHit>& hitsOnTrack = foundTracks[i];
            if(hitsOnTrack.empty()) continue;

            Bool_t isChargedHadronInTrack = IsChargedHadron(hitsOnTrack[0]);
            if(!isChargedHadronInTrack) continue;

            for(size_t j = 0; j < hitsOnTrack.size(); ++j)
            {   
                ReconHit& hit = hitsOnTrack[j];
                totalEdepInChargedHadrons += hit.fEdep;
            }
        }
        mcEventRecord.SetHadronEdep(totalEdepInChargedHadrons);
    }

    //4. Is it is an electron, count the total charge particles exiting the detector
    if(mcEventRecord.IsPrimaryLeptonElectron())
    {
        Int_t numOfExitingChargedParticles = 0;
        std::vector<Int_t> pdgCodes;
        for(size_t i = 0; i <  foundTracks.size() ; ++i)
        {
            std::vector<ReconHit>& hitsOnTrack = foundTracks[i];
            if(hitsOnTrack.empty()) continue;

            Bool_t isChargedParticleInTrack = IsChargedParticle(hitsOnTrack[0]);
            if(!isChargedParticleInTrack) continue;

            Bool_t isParExiting = IsHitExiting(hitsOnTrack[hitsOnTrack.size() - 1]);
            if(isParExiting)
            {
                ++numOfExitingChargedParticles;
                pdgCodes.push_back(hitsOnTrack[0].fpdg);
            }
        }
        mcEventRecord.SetNumOfExitingPar(numOfExitingChargedParticles);
        mcEventRecord.SetPdgOfExitingPars(pdgCodes);
    }

    // 5. Fit the muon momentum
    if(ffitMuonMom && mcEventRecord.IsPrimaryLeptonMuon())
    {
        for(size_t i = 0; i <  foundTracks.size() ; ++i)
        {
            std::vector<ReconHit>& hitsOnTrack = foundTracks[i];
            if(hitsOnTrack.empty()) continue;
            if(hitsOnTrack[0].fpdg == genie::kPdgMuon || hitsOnTrack[0].fpdg == genie::kPdgAntiMuon)
            {
                TVector3 fitMom;
                if(FitTrack(hitsOnTrack, fitMom))
                {
                    mcEventRecord.SetGenfitMom(fitMom);

                    TVector3 mcMom = mcEventRecord.GetMuonMom();
                    Double_t err = mcMom.Mag() - fitMom.Mag();
                    mcEventRecord.SetMomError(err);
                }
                break;
            }
        }
    }

    // 6. Calculate proton energy loss from the generated photons
    if(mcEventRecord.IsPrimaryLeptonMuon() || mcEventRecord.IsPrimaryLeptonElectron())
    {
        for(size_t i = 0; i <  foundTracks.size() ; ++i)
        {
            std::vector<ReconHit>& hitsOnTrack = foundTracks[i];
            if(hitsOnTrack.empty()) continue;

            Int_t& pdgPar = hitsOnTrack[0].fpdg;
            if(genie::pdg::IsProton(pdgPar))
            {
                Double_t sumEdep = 0;
                for(size_t j = 0; j < hitsOnTrack.size(); ++j)
                {   
                    ReconHit& hit = hitsOnTrack[j];
                    sumEdep += CalculatePhotoEdep(hit);
                }
                mcEventRecord.SetProtonEdep(sumEdep);
                break;
            }
        }
    }

    // 7. Determine the muon dedx = a*p + b coeff
    if(mcEventRecord.IsPrimaryLeptonMuon())
    {
        for(size_t i = 0; i <  foundTracks.size() ; ++i)
        {
            std::vector<ReconHit>& hitsOnTrack = foundTracks[i];
            if(hitsOnTrack.empty()) continue;
            if(hitsOnTrack[0].fpdg == genie::kPdgMuon || hitsOnTrack[0].fpdg == genie::kPdgAntiMuon)
            {
                Double_t dedx = RevertToDeDxMC(hitsOnTrack);
                mcEventRecord.SetDeDx(dedx);

                Double_t edepdedx = RevertEdepToDeDx(hitsOnTrack);
                mcEventRecord.SetEdepDeDx(edepdedx);
                break;
            }
        }
    }

    // 8. Fill in data for particles exiting the detector
    if(foutputFile.is_open())
    {
        Int_t nuPdg = mcEventRecord.GetNuPdg();
        Double_t nuE = mcEventRecord.GetNuE();
        Bool_t isweakcc = mcEventRecord.IsWeakCC(); 
		Bool_t isweaknc = mcEventRecord.IsWeakNC();
		Bool_t isqes = mcEventRecord.IsQuasiElastic();
        foutputFile << feventNum << " " <<nuPdg << " " << nuE << " " << " " << isweakcc << " " << " " << isweaknc << " " << " " << isqes << " ";

        FgdExitData exitDat(nuPdg, nuE, isweakcc, isweaknc, isqes , {});

        bool hasExiting(false);
        for(size_t i = 0; i <  foundTracks.size() ; ++i)
        {
            std::vector<ReconHit>& hitsOnTrack = foundTracks[i];
            if(hitsOnTrack.empty()) continue;
            ReconHit& lastHit = hitsOnTrack[hitsOnTrack.size() -1 ];
            if(IsHitExiting(lastHit) && lastHit.fmomExit.Mag()!=0)
            {  
                foutputFile  << lastHit.fpdg << " " << lastHit.fmomExit.X() << " " <<  lastHit.fmomExit.Y() << " " << lastHit.fmomExit.Z() 
                                << " " << lastHit.fMCPos.X() << " " << lastHit.fMCPos.Y() << " " << lastHit.fMCPos.Z() << " ";  
                hasExiting = true;

                exitDat.fVecPars.emplace_back(FgdExitParticle(lastHit.fpdg, lastHit.fmomExit, lastHit.fMCPos));
            }
        }

        exitDat.fIsPrimaryMuon = mcEventRecord.IsPrimaryLeptonMuon();
        exitDat.fIsPrimaryElectron = mcEventRecord.IsPrimaryLeptonElectron();
        exitDat.fHasExiting = hasExiting;
        exitDat.fEventNum = feventNum;
        
        if(mcEventRecord.IsPrimaryLeptonMuon())
        {
            exitDat.fPrimaryLeptonMom = mcEventRecord.GetMuonMom();
        } 
        else if(mcEventRecord.IsPrimaryLeptonElectron())
        {
            exitDat.fPrimaryLeptonMom = mcEventRecord.GetElectronMom();
        } 

        Int_t sumTotalCubes = 0;
        TVector3 sumTotalPhoto(0,0,0);
        Float_t Maxtrack(0.);
        Int_t numOfTracks = 0;

        TH1F* ph_hist = new TH1F("hist_ph","Photons per cube histogram",50,0,300);
        for(size_t i = 0; i <  foundTracks.size() ; ++i)
        {
            std::vector<ReconHit>& track = foundTracks[i];
            if(isParticleNeutral(track[0].fpdg))
            {
                continue;
            }

            Float_t trackLenght = track[track.size()-1].ftrackLengthOrigin;
            if(Maxtrack < trackLenght)
            {
                Maxtrack = trackLenght;
            }

            if(MIN_CUBES_LENGTH_FOR_TRACK < track.size())
            {
                ++numOfTracks;
            }

            for(size_t j = 0; j <  track.size() ; ++j)
            {
                ReconHit& hit = track[j];
                sumTotalPhoto +=hit.fphotons;
                float tempSum = hit.fphotons.X() + hit.fphotons.Y() + hit.fphotons.Z();
                ph_hist->Fill(tempSum);
                ++sumTotalCubes;
            } 
        }

        Double_t mean = ph_hist->GetMean();
        Double_t stdDev = ph_hist->GetStdDev();
        Double_t maxVal = ph_hist->GetBinContent(ph_hist->GetMaximumBin());

        exitDat.fmean = mean;
        exitDat.fstdDev = stdDev;
        exitDat.fmaxVal = maxVal;

        exitDat.fTotalPhotons = sumTotalPhoto;
        exitDat.fTotalCubes = sumTotalCubes;
        exitDat.fMaxtrack = Maxtrack;
        exitDat.fnumOfTracks = numOfTracks;

        fExitData.push_back(exitDat);

        if(!hasExiting)
        {
            foutputFile << 0 << " " << 0 << " " << 0;
        }
        foutputFile << endl;
    }
    

    feventRecords[feventNum].SetHasHits(true);
    ++feventNum; // Increment to next event from eventData read from simulation`s genie export
}

Bool_t FgdMCLeptonStats::IsHitExiting(ReconHit& hit)
{
    Int_t lastMppc = f_bin_Z - 1;
    return (hit.fmppcLoc.Z() ==  lastMppc);
}

Bool_t FgdMCLeptonStats::IsChargedHadron(ReconHit& hit)
{
    bool rc(false);

    if(genie::pdg::IsHadron(hit.fpdg))
    {
        TParticlePDG* tPdg = fpdgDB->GetParticle(hit.fpdg);
        rc = (tPdg!=nullptr) && (tPdg->Charge()!=0);
    }

    return rc;
}

Bool_t FgdMCLeptonStats::IsChargedParticle(ReconHit& hit)
{
    bool rc(false);

    TParticlePDG* tPdg = fpdgDB->GetParticle(hit.fpdg);
    rc = (tPdg!=nullptr) && (tPdg->Charge()!=0);

    return rc;
}

Double_t FgdMCLeptonStats::CalculatePhotoEdep(ReconHit& hit)
{
    using namespace esbroot::digitizer::superfgd;

    TVector3& pe_1_dir = hit.fph1;
    TVector3& dist_1 = hit.fmppc1;
    TVector3& pe_2_dir = hit.fph2;
    TVector3& dist_2 = hit.fmppc2;

    double& time = hit.ftime;
    double charge = 1.0; // not used, but is a parameter from legacy


    double pe_1_x = FgdDigitizer::RevertyMPPCResponse(pe_1_dir.X());
    double pe_1_y = FgdDigitizer::RevertyMPPCResponse(pe_1_dir.Y());
    double pe_1_Z = FgdDigitizer::RevertyMPPCResponse(pe_1_dir.Z());

    double pe_2_x = FgdDigitizer::RevertyMPPCResponse(pe_2_dir.X());
    double pe_2_y = FgdDigitizer::RevertyMPPCResponse(pe_2_dir.Y());
    double pe_2_z = FgdDigitizer::RevertyMPPCResponse(pe_2_dir.Z());

    FgdDigitizer::RevertFiberResponse(pe_1_x, time, dist_1.X());
    FgdDigitizer::RevertFiberResponse(pe_1_y, time, dist_1.Y());
    FgdDigitizer::RevertFiberResponse(pe_1_Z, time, dist_1.Z());

    FgdDigitizer::RevertFiberResponse(pe_2_x, time, dist_2.X());
    FgdDigitizer::RevertFiberResponse(pe_2_y, time, dist_2.Y());
    FgdDigitizer::RevertFiberResponse(pe_2_z, time, dist_2.Z());

    // Deposited energy hit.fEdep and tracklength hit.ftrackLength
    // are used to calculate the CBIRKS coefficients together with dedx (energy losses)
    // to be able to revert from the photons

    Double_t x_1 = FgdDigitizer::RevertScintiResponse(hit.fEdep, hit.ftrackLength, charge, pe_1_x);
    Double_t y_1 = FgdDigitizer::RevertScintiResponse(hit.fEdep, hit.ftrackLength, charge, pe_1_y);
    Double_t z_1 = FgdDigitizer::RevertScintiResponse(hit.fEdep, hit.ftrackLength, charge, pe_1_Z);

    Double_t x_2 = FgdDigitizer::RevertScintiResponse(hit.fEdep, hit.ftrackLength, charge, pe_2_x);
    Double_t y_2 = FgdDigitizer::RevertScintiResponse(hit.fEdep, hit.ftrackLength, charge, pe_2_y);
    Double_t z_2 = FgdDigitizer::RevertScintiResponse(hit.fEdep, hit.ftrackLength, charge, pe_2_z);

    Double_t x = x_1 + x_2;
    Double_t y = y_1 + y_2;
    Double_t z = z_1 + z_2;

    Double_t totalEdep =  x + y + z;
    return totalEdep;
}

Bool_t FgdMCLeptonStats::FitTrack(std::vector<ReconHit>& hitsOnTrack, TVector3& fitMom)
{
    bool rc(true);

    // init geometry and mag. field
    TVector3 magField = fgdConstructor.GetMagneticField(); // values are in kGauss
    genfit::FieldManager::getInstance()->init(new genfit::ConstField(magField.X(),magField.Y(), magField.Z())); 
    genfit::MaterialEffects::getInstance()->init(new genfit::TGeoMaterialInterface());
    genfit::MaterialEffects::getInstance()->setDebugLvl(fDebuglvl_genfit);

    // init fitter
    std::shared_ptr<genfit::AbsKalmanFitter> fitter = make_shared<genfit::KalmanFitterRefTrack>();
    //std::shared_ptr<genfit::AbsKalmanFitter> fitter = make_shared<genfit::KalmanFitter>();
    fitter->setMinIterations(fminGenFitInterations);
    fitter->setMaxIterations(fmaxGenFitIterations);
    fitter->setDebugLvl(fDebuglvl_genfit);

    int detId(1); // Detector id, it is the same, we only have one detector

    const int pdg = hitsOnTrack[0].fpdg;
    TVector3 posM(hitsOnTrack[0].fHitPos);
    TVector3 momM(hitsOnTrack[0].fmom);

    if(isParticleNeutral(pdg))
    {
        LOG(debug) << "Track is of neutral particle ["<< pdg << "] continue with next track.";
        return false;
    }

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

    LOG(debug) << "******************************************* ";
    LOG(debug) << " \tPdg code " << pdg;
    LOG(debug) << " \tHits in track "<< hitsOnTrack.size();
    LOG(debug) << " \tTrack Momentum [" << momM.Mag() << "]" << "(" << momM.X() << "," << momM.Y() << "," << momM.Z() << ")";
    
    //std::vector<genfit::AbsMeasurement*> measurements;
    for(Int_t bh = 0; bh < hitsOnTrack.size(); ++bh)
    {
        TVectorD hitPos(3);
        hitPos(0) = hitsOnTrack[bh].fHitPos.X();
        hitPos(1) = hitsOnTrack[bh].fHitPos.Y();
        hitPos(2) = hitsOnTrack[bh].fHitPos.Z();

        genfit::AbsMeasurement* measurement = new genfit::SpacepointMeasurement(hitPos, hitCov, detId, bh, nullptr);
        toFitTrack->insertMeasurement(measurement, bh);
        //measurements.emplace_back(measurement);
    }
    //toFitTrack->insertPoint(new genfit::TrackPoint(measurements, toFitTrack));

    try
    {
        //check
        toFitTrack->checkConsistency();

        // do the fit
        fitter->processTrack(toFitTrack, true);

        //check
        toFitTrack->checkConsistency();

        PrintFitTrack(*toFitTrack);
        genfit::FitStatus* fiStatuStatus = toFitTrack->getFitStatus();

        // WriteOutput(  pdg
        //             , (*toFitTrack).getFittedState().getMom()
        //             , momM
        //             , *toFitTrack
        //             , fiStatuStatus);

        fitMom = (*toFitTrack).getFittedState().getMom();

        LOG(debug) <<"******************************************* ";
    }
    catch(genfit::Exception& e)
    {
        LOG(error) <<"Exception, when tryng to fit track";
        LOG(error) << e.what();
        LOG(error) << e.getExcString();
        rc = false;
    }

    return rc;
}

void FgdMCLeptonStats::afterRun()
{
    TFile * outFile = new TFile(foutputRootFile.c_str(), "RECREATE", "Reconstruction data from Fgd Detector");
    outFile->SetCompressionLevel(9);

    FgdMCEventRecord* data = nullptr;
    TTree * outTree = new TTree(esbroot::geometry::superfgd::DP::FGD_MC_LEPTON_RECONSTRUCTION_TTREE.c_str()
                                ,esbroot::geometry::superfgd::DP::FGD_MC_LEPTON_RECONSTRUCTION_ROOT_FILE.c_str());
    outTree->Branch(esbroot::geometry::superfgd::DP::FGD_MC_LEPTON_RECONSTRUCTION_BRANCH.c_str(), &data);

    // TClonesArray& clref = *fEventsArray;

    for(size_t ind = 0 ; ind < feventRecords.size(); ind++)
    {
        // LOG(info) << "writing to output file " << ind;
        // auto test = new(clref[clref.GetEntries()]) esbroot::reconstruction::superfgd::FgdMCEventRecord(feventRecords[ind]);
        // LOG(info) << "Is null " << (test == nullptr);

        data = &feventRecords[ind];
        outTree->Fill();
    }


    Float_t totalPhotons(0.);
    Float_t totalCubes(0);
    Float_t maxtrack(0.);
    Float_t numOfTracks(0);
    Float_t maxVal(0.);
    Float_t stdDev(0.);
    Float_t hasExiting(0.); 
    Float_t eventNum(0.); 

    FgdExitData* exitData = nullptr;
    TTree * exitParsTree = new TTree(esbroot::geometry::superfgd::DP::FGD_MC_EXIT_PARTICLES_RECONSTRUCTION_TTREE.c_str()
                                ,esbroot::geometry::superfgd::DP::FGD_MC_EXIT_PARTICLES_RECONSTRUCTION_ROOT_FILE.c_str());
    exitParsTree->Branch(esbroot::geometry::superfgd::DP::FGD_MC_EXIT_PARTICLES_RECONSTRUCTION_BRANCH.c_str(), &exitData);
    exitParsTree->Branch("totalPhotons", &totalPhotons);
    exitParsTree->Branch("totalCubes", &totalCubes);
    exitParsTree->Branch("maxtrack", &maxtrack);
    exitParsTree->Branch("numOfTracks", &numOfTracks);
    exitParsTree->Branch("maxVal", &maxVal);
    exitParsTree->Branch("stdDev", &stdDev);
    exitParsTree->Branch("hasExiting", &hasExiting);
    exitParsTree->Branch("eventNum", &eventNum);
    for(size_t ind = 0 ; ind < fExitData.size(); ind++)
    {
        exitData = &fExitData[ind];
        totalPhotons = exitData->fTotalPhotons.X() + exitData->fTotalPhotons.Y() + exitData->fTotalPhotons.Z();
        totalCubes = exitData->fTotalCubes;
        maxtrack = exitData->fMaxtrack;
        numOfTracks = exitData->fnumOfTracks;
        maxVal = exitData->fmaxVal;
        stdDev = exitData->fstdDev;
        hasExiting = static_cast<Float_t>(exitData->fHasExiting);
        eventNum = exitData->fEventNum;
        exitParsTree->Fill();
    }

    
    

    TTree * exitElectronParsTree = new TTree(esbroot::geometry::superfgd::DP::FGD_MC_EXIT_PARTICLES_RECONSTRUCTION_TTREE_ELECTRON.c_str()
                                ,esbroot::geometry::superfgd::DP::FGD_MC_EXIT_PARTICLES_RECONSTRUCTION_ROOT_FILE.c_str());
    exitElectronParsTree->Branch(esbroot::geometry::superfgd::DP::FGD_MC_EXIT_PARTICLES_RECONSTRUCTION_BRANCH_ELECTRON.c_str(), &exitData);
    exitElectronParsTree->Branch("totalPhotons", &totalPhotons);
    exitElectronParsTree->Branch("totalCubes", &totalCubes);
    exitElectronParsTree->Branch("maxtrack", &maxtrack);
    exitElectronParsTree->Branch("numOfTracks", &numOfTracks);
    exitElectronParsTree->Branch("maxVal", &maxVal);
    exitElectronParsTree->Branch("stdDev", &stdDev);
    exitElectronParsTree->Branch("hasExiting", &hasExiting);
    exitElectronParsTree->Branch("eventNum", &eventNum);
    for(size_t ind = 0 ; ind < fExitData.size(); ind++)
    {
        exitData = &fExitData[ind];
         
        if(exitData->fIsPrimaryElectron)
        {
            totalPhotons = exitData->fTotalPhotons.X() + exitData->fTotalPhotons.Y() + exitData->fTotalPhotons.Z();
            totalCubes = exitData->fTotalCubes;
            maxtrack = exitData->fMaxtrack;
            numOfTracks = exitData->fnumOfTracks;
            maxVal = exitData->fmaxVal;
            stdDev = exitData->fstdDev;
            hasExiting = static_cast<Float_t>(exitData->fHasExiting);
            eventNum = exitData->fEventNum;
            exitElectronParsTree->Fill();
        }
        
    }

    TTree * exitMuonParsTree = new TTree(esbroot::geometry::superfgd::DP::FGD_MC_EXIT_PARTICLES_RECONSTRUCTION_TTREE_MUON.c_str()
                                ,esbroot::geometry::superfgd::DP::FGD_MC_EXIT_PARTICLES_RECONSTRUCTION_ROOT_FILE.c_str());
    exitMuonParsTree->Branch(esbroot::geometry::superfgd::DP::FGD_MC_EXIT_PARTICLES_RECONSTRUCTION_BRANCH_MUON.c_str(), &exitData);
    exitMuonParsTree->Branch("totalPhotons", &totalPhotons);
    exitMuonParsTree->Branch("totalCubes", &totalCubes);
    exitMuonParsTree->Branch("maxtrack", &maxtrack);
    exitMuonParsTree->Branch("numOfTracks", &numOfTracks);
    exitMuonParsTree->Branch("maxVal", &maxVal);
    exitMuonParsTree->Branch("stdDev", &stdDev);
    exitMuonParsTree->Branch("hasExiting", &hasExiting);
    exitMuonParsTree->Branch("eventNum", &eventNum);
    for(size_t ind = 0 ; ind < fExitData.size(); ind++)
    {
        exitData = &fExitData[ind];
        if(exitData->fIsPrimaryMuon)
        {
            totalPhotons = exitData->fTotalPhotons.X() + exitData->fTotalPhotons.Y() + exitData->fTotalPhotons.Z();
            totalCubes = exitData->fTotalCubes;
            maxtrack = exitData->fMaxtrack;
            numOfTracks = exitData->fnumOfTracks;
            maxVal = exitData->fmaxVal;
            stdDev = exitData->fstdDev;
            hasExiting = static_cast<Float_t>(exitData->fHasExiting);
            eventNum = exitData->fEventNum;
            exitMuonParsTree->Fill();
        }
        
    }

    outFile->WriteTObject(outTree);
    outFile->WriteTObject(exitParsTree);
    outFile->WriteTObject(exitElectronParsTree);
    outFile->WriteTObject(exitMuonParsTree);
    outFile->Close();
    
    delete outFile;

    FgdMCGenFitRecon::afterRun();


    // int zeroLenghtMuionTracks = 0;
    // double z_min = 100000.f;
    // double z_max = -100000.f;
    // for(size_t i =0; i< feventRecords.size();++i)
    // {
    //     if(feventRecords[i].IsPrimaryLeptonMuon() && feventRecords[i].GetMuonTrackLengthOrigin() == 0)
    //     {
    //         ++zeroLenghtMuionTracks;
    //     }   
    //     LOG(info) << feventRecords[i].IsPrimaryLeptonMuon();
    //     LOG(info) << feventRecords[i].GetMuonTrackLength();
    //     LOG(info) << feventRecords[i].GetMuonTrackLengthOrigin();
    //     LOG(info) << feventRecords[i].IsMuonExiting();
    //     LOG(info) << feventRecords[i].HasHits();
    //     LOG(info) << "X " << feventRecords[i].GetVertex().X() << " Y " << feventRecords[i].GetVertex().Y() << " Z " << feventRecords[i].GetVertex().Z();
    //     LOG(info) << " ==================== ";

    //     if(z_min > feventRecords[i].GetVertex().Z()) z_min = feventRecords[i].GetVertex().Z();
    //     if(z_max < feventRecords[i].GetVertex().Z()) z_max = feventRecords[i].GetVertex().Z();
    // }
    // LOG(info) << "Zero length muons tracks " << zeroLenghtMuionTracks;
}
// -------------------------------------------------------------------------


// -----   Protected methods   --------------------------------------------

// -------------------------------------------------------------------------

}// namespace superfgd
}// namespace reconstruction
}// namespace esbroot
