#include "EsbReconstruction/EsbSuperFGD/FgdTMVAEventRecord.h"

#include "Framework/ParticleData/PDGCodes.h"
#include "FairLogger.h"

#include <vector>
#include <set>
#include <fstream>
#include <iostream>
#include <sstream>
#include <memory>
#include <math.h>
#include <bits/stdc++.h>

namespace esbroot {
namespace reconstruction {
namespace superfgd {


// -----   Constructors and Destructors  -----------------------------------

FgdTMVAEventRecord::FgdTMVAEventRecord(std::string eventData) 
    : TObject(), feventData(eventData), fnuPdg(0)
        , fNuEnergy(0.), fvertex(TVector3(0,0,0)), fIsWeakCC(false)
        , fIsWeakNC(false), fIsQuasiElastic(false), fIsDeepInElastic(false)
        , fIsResonant(false), fIsCoherentProduction(false), fIsMEC(false)
        , fIsNuElectronElastic(false), fIsElectronScattering(false)
        , fIsPrimaryMuon(false)
        , fIsPrimaryElectron(false), fPrimaryMuonMom(TVector3(0,0,0))
        , fPrimaryElectronMom(TVector3(0,0,0)), fMuonTrackLength(0.) , fMuonTrackLengthOrigin(0.)
        , fIsMuonExiting(false), fMuonExitMomentum(TVector3(0,0,0))
        , fMuonPolarAngle(0.), fMuonAzumAngle(0.), fElectronNumOfExitingParticles(0)
        , fTotalEdep(0.), fTotalPhotons(0,0,0), fTotalCubes(0)
        , fHasHits(false), fTrueEdep(0), fpe(0)
        , fPrimaryMuonFitMom(TVector3(0,0,0))
        , fPrimaryMuonCalMom(TVector3(0,0,0))
        , fEventType(EventType::UNDEFINED)
{
    Init();
}

FgdTMVAEventRecord::FgdTMVAEventRecord() 
    : feventData("")
{
}

FgdTMVAEventRecord::FgdTMVAEventRecord(const FgdTMVAEventRecord& c)
{
    this->feventData = c.feventData;
    
    this->fMuonTrackLength = c.fMuonTrackLength;
    this->fIsMuonExiting = c.fIsMuonExiting;
    this->fMuonExitMomentum = c.fMuonExitMomentum;
    this->fMuonPolarAngle = c.fMuonPolarAngle;
    this->fMuonAzumAngle = c.fMuonAzumAngle;
    this->fTotalEdep = c.fTotalEdep;
    this->fElectronNumOfExitingParticles = c.fElectronNumOfExitingParticles;
    this->fTotalPhotons = c.fTotalPhotons;
    this->fTotalCubes = c.fTotalCubes;
    this->fHasHits = c.fHasHits;
    this->fTrueEdep = c.fTrueEdep;
    this->fpe = c.fpe;
    this->fPrimaryMuonFitMom = c.fPrimaryMuonFitMom;
    this->fPrimaryMuonCalMom = c.fPrimaryMuonCalMom;
    this->fMuonTrackLengthOrigin = c.fMuonTrackLengthOrigin;

    this->Init();
}

FgdTMVAEventRecord::~FgdTMVAEventRecord()
{
}
// -------------------------------------------------------------------------

// -----   Public methods   ------------------------------------------------

void FgdTMVAEventRecord::SetEventData(std::string data)
{
    feventData = data;

    Init();
}

Int_t FgdTMVAEventRecord::GetNuPdg(void)
{
    return fnuPdg;
}

Double_t FgdTMVAEventRecord::GetNuE(void)
{
    return fNuEnergy;
}

TVector3 FgdTMVAEventRecord::GetVertex(void)
{
    return fvertex;
}

Bool_t FgdTMVAEventRecord::IsWeakCC(void)
{
    return fIsWeakCC;
}

Bool_t FgdTMVAEventRecord::IsWeakNC(void)
{
    return fIsWeakNC;
}

Bool_t FgdTMVAEventRecord::IsQuasiElastic(void)
{
    return fIsQuasiElastic;
}

Bool_t FgdTMVAEventRecord::IsDeepInElastic(void)
{
    return fIsDeepInElastic;
}

Bool_t FgdTMVAEventRecord::IsResonant(void)
{
    return fIsResonant;
}

Bool_t FgdTMVAEventRecord::IsCoherentProduction(void)
{
    return fIsCoherentProduction;
}

Bool_t FgdTMVAEventRecord::IsMEC(void)
{
    return fIsMEC;
}

Bool_t FgdTMVAEventRecord::IsNuElectronElastic(void)
{
    return fIsNuElectronElastic;
}  

Bool_t FgdTMVAEventRecord::IsElectronScattering(void)
{
    return fIsElectronScattering;
}

Bool_t FgdTMVAEventRecord::HasHits(void)
{
    return fHasHits;
}


Bool_t FgdTMVAEventRecord::IsPrimaryLeptonMuon()
{
    return fIsPrimaryMuon;
}

TVector3 FgdTMVAEventRecord::GetMuonMom()
{
    return fPrimaryMuonMom;
}

Double_t FgdTMVAEventRecord::GetMuonTrackLength()
{
    return fMuonTrackLength;
}


Double_t FgdTMVAEventRecord::GetMuonTrackLengthOrigin()
{
    return fMuonTrackLengthOrigin;
}

Bool_t FgdTMVAEventRecord::IsPrimaryLeptonElectron()
{
    return fIsPrimaryElectron;
}

TVector3 FgdTMVAEventRecord::GetElectronMom()
{
    return fPrimaryElectronMom;
}

Double_t FgdTMVAEventRecord::GetMuonPolarAngle()
{
    return fMuonPolarAngle;
}

Double_t FgdTMVAEventRecord::GetMuonAzumuteAngle()
{
    return fMuonAzumAngle;
}

void FgdTMVAEventRecord::SetMuonExitMom(TVector3 exitMom)
{
    fMuonExitMomentum = exitMom;

    Double_t radToAngle = 180/TMath::Pi();
    fMuonPolarAngle = fMuonExitMomentum.Theta() * radToAngle;
    fMuonAzumAngle = fMuonExitMomentum.Phi() * radToAngle;
}


FgdTMVAEventRecord& FgdTMVAEventRecord::operator=(const FgdTMVAEventRecord& c)
{
    this->feventData = c.feventData;

    this->fMuonTrackLength = c.fMuonTrackLength;
    this->fIsMuonExiting = c.fIsMuonExiting;
    this->fMuonExitMomentum = c.fMuonExitMomentum;
    this->fMuonPolarAngle = c.fMuonPolarAngle;
    this->fMuonAzumAngle = c.fMuonAzumAngle;
    this->fTotalEdep = c.fTotalEdep;
    this->fElectronNumOfExitingParticles = c.fElectronNumOfExitingParticles;
    this->fTotalPhotons = c.fTotalPhotons;
    this->fTotalCubes = c.fTotalCubes;
    this->fHasHits = c.fHasHits;
    this->fTrueEdep = c.fTrueEdep;
    this->fpe = c.fpe;

    this->fPrimaryMuonFitMom = c.fPrimaryMuonFitMom;
    this->fPrimaryMuonCalMom = c.fPrimaryMuonCalMom;
    this->fMuonTrackLengthOrigin = c.fMuonTrackLengthOrigin;

    this->Init();
    return *this;
}

const std::vector<std::pair<Int_t, TVector3>>& FgdTMVAEventRecord::GetPrimaryParticles()
{
    Init();

    return fPrimaryParticles;
}

void FgdTMVAEventRecord::PrintData(std::ostream & stream)
{
    stream << feventData;
}

void FgdTMVAEventRecord::ReadEventData()
{
    this->Init();
}

// -------------------------------------------------------------------------


// -----   Protected methods   ---------------------------------------------
void FgdTMVAEventRecord::Init()
{
    fDataTokens.clear();

    try
    {        
        const char spaceChar(' ');

        std::istringstream ss(feventData);
        std::string token;
        while(std::getline(ss, token, spaceChar))
        {
            if(!token.empty())
            {
                fDataTokens.push_back(token);
            }
        }
        InitMembers();
    }
    catch(const std::exception& e)
    {
        LOG(fatal) << e.what();
    }
}

void FgdTMVAEventRecord::InitMembers()
{
    fnuPdg = std::stoi(fDataTokens[Data::NEUTRINO_PDG]);
    fNuEnergy = std::stod(fDataTokens[Data::NEUTRINO_ENERGY]);

    Double_t x = std::stod(fDataTokens[Data::VERTEX_POSTION_X]);
    Double_t y = std::stod(fDataTokens[Data::VERTEX_POSTION_Y]);
    Double_t z = std::stod(fDataTokens[Data::VERTEX_POSTION_Z]);

    fvertex = TVector3(x,y,z);

    Int_t isWeakCC = std::stoi(fDataTokens[Data::IS_WEAK_CC]);
    fIsWeakCC = (isWeakCC==1);

    Int_t isWeakNC = std::stoi(fDataTokens[Data::IS_WEACK_NC]);
    fIsWeakNC = (isWeakNC==1);

    Int_t isQuasiElastic = std::stoi(fDataTokens[Data::IS_QUASI_ELASTIC]);
    fIsQuasiElastic = (isQuasiElastic==1);

    Int_t isDIS = std::stoi(fDataTokens[Data::IS_DEEP_INELASRIC]);
    fIsDeepInElastic = (isDIS==1);

    Int_t isResonant = std::stoi(fDataTokens[Data::IS_RESONANT]);
    fIsResonant = (isResonant==1);

    Int_t isCohPro = std::stoi(fDataTokens[Data::IS_COHERENT_PRODUCTION]);
    fIsCoherentProduction = (isCohPro==1);

    Int_t isMec = std::stoi(fDataTokens[Data::IS_MEC]);
    fIsMEC = (isMec==1);

    Int_t isNuElEl = std::stoi(fDataTokens[Data::IS_NuElectron_Elastic]);
    fIsNuElectronElastic = (isNuElEl==1); 

    Int_t isElEl = std::stoi(fDataTokens[Data::IS_ELECTRON_SCATTERING]);
    fIsElectronScattering = (isElEl==1); 

    // Initalize primary particles
    size_t particleDataSize = 4;

    size_t particlePdg = 0;
    size_t particleMomX = 1;
    size_t particleMomY = 2;
    size_t particleMomZ = 3;

    fPrimaryParticles.clear();
    for(size_t i = Data::PRIMARY_PARTICLES; i < fDataTokens.size(); i+=particleDataSize)
    {
        Int_t pdg = std::stoi(fDataTokens[i + particlePdg]);
        Double_t momX = std::stod(fDataTokens[i + particleMomX]);
        Double_t momY = std::stod(fDataTokens[i + particleMomY]);
        Double_t momZ = std::stod(fDataTokens[i + particleMomZ]);

        fPrimaryParticles.push_back(
                            std::pair<Int_t, TVector3>( pdg,    TVector3(momX , momY, momZ) )
                            );
    }

    for(size_t i = 0; i< fPrimaryParticles.size(); ++i)
    {
        std::pair<Int_t, TVector3>& particle = fPrimaryParticles[i];
        if(particle.first == genie::kPdgMuon || particle.first == genie::kPdgAntiMuon)
        {
            fIsPrimaryMuon = true;
            fPrimaryMuonMom = particle.second;
            break;
        }

        if(particle.first == genie::kPdgElectron || particle.first == genie::kPdgPositron)
        {
            fIsPrimaryElectron = true;
            fPrimaryElectronMom = particle.second;
            break;
        }
    }


    fillType();
    determineEventType();
}


void FgdTMVAEventRecord::fillType()
{
    fparticleTypes[genie::kPdgMuon] = 0;
    fparticleTypes[genie::kPdgAntiMuon] = 0;
    fparticleTypes[genie::kPdgProton] = 0;
    fparticleTypes[genie::kPdgPiP] = 0;
    fparticleTypes[genie::kPdgPiM] = 0;
    fparticleTypes[genie::kPdgElectron] = 0;
    fparticleTypes[genie::kPdgPositron] = 0;
    

    for(size_t i = 0; i< fPrimaryParticles.size(); ++i)
    {
        std::pair<Int_t, TVector3>& particle = fPrimaryParticles[i];
        auto iter = fparticleTypes.find(particle.first);
        if(iter!= fparticleTypes.end())
        {
            ++(iter->second);
        }
    }
}

size_t FgdTMVAEventRecord::numOfParTypes(std::map<Int_t, Int_t>& map, std::set<Int_t>& keys, bool countAllExceptKeys)
{
    auto iter = map.begin();
    size_t res(0);
    while(iter != map.end())
    {
        if(!countAllExceptKeys && keys.find(iter->first) != keys.end())
        {
            res += iter->second;
        }
        else if(countAllExceptKeys && keys.find(iter->first) == keys.end())
        {
            res += iter->second;
        }
        ++iter;
    }
    return res;
}

void FgdTMVAEventRecord::determineEventType()
{
    const static size_t SINGLE = 1;
    const static size_t NONE = 0;

    std::set<Int_t> pdgKeys;

    // ======================================================
    // 1. Muon only
    {
        pdgKeys.insert(genie::kPdgMuon);

        size_t muons = numOfParTypes(fparticleTypes,pdgKeys, false);
        size_t allExceptMuons = numOfParTypes(fparticleTypes,pdgKeys, true);

        if(muons == SINGLE && allExceptMuons == NONE) {fEventType = EventType::MUON_ONLY; return;}
        pdgKeys.clear();
    }
    // ======================================================

    // ======================================================
    // 2. AntiMuon only
    {
        pdgKeys.insert(genie::kPdgAntiMuon);

        size_t antimuons = numOfParTypes(fparticleTypes,pdgKeys, false);
        size_t allExceptAntiMuons = numOfParTypes(fparticleTypes,pdgKeys, true);

        if(antimuons == SINGLE && allExceptAntiMuons == NONE) {fEventType = EventType::ANTI_MUON_ONLY; return;}
        pdgKeys.clear();
    }
    // ======================================================

    // ======================================================
    // 3. Muon and proton only
    {
        std::set<Int_t> pdgKeys;
        pdgKeys.insert(genie::kPdgMuon);

        size_t muons = numOfParTypes(fparticleTypes,pdgKeys, false);
        pdgKeys.clear();
        pdgKeys.insert(genie::kPdgProton);
        size_t protons = numOfParTypes(fparticleTypes,pdgKeys, false);
        pdgKeys.clear();
        pdgKeys.insert(genie::kPdgMuon);
        pdgKeys.insert(genie::kPdgProton);
        size_t allExceptMuonProton = numOfParTypes(fparticleTypes,pdgKeys, true);

        if(muons == SINGLE && protons == SINGLE && allExceptMuonProton == NONE) {fEventType = EventType::MUON_AND_PROTON_ONLY; return;}
        pdgKeys.clear();
    }
    // ======================================================

    // ======================================================
    // 4. Anti Muon and proton only
    {
        std::set<Int_t> pdgKeys;
        pdgKeys.insert(genie::kPdgAntiMuon);

        size_t antimuons = numOfParTypes(fparticleTypes,pdgKeys, false);
        pdgKeys.clear();
        pdgKeys.insert(genie::kPdgProton);
        size_t protons = numOfParTypes(fparticleTypes,pdgKeys, false);
        pdgKeys.clear();
        pdgKeys.insert(genie::kPdgAntiMuon);
        pdgKeys.insert(genie::kPdgProton);
        size_t allExceptMuonProton = numOfParTypes(fparticleTypes,pdgKeys, true);

        if(antimuons == SINGLE && protons == SINGLE && allExceptMuonProton == NONE) {fEventType = EventType::ANTI_MUON_AND_PROTON_ONLY; return;}
        pdgKeys.clear();
    }
    // ======================================================

    // ======================================================
    // 5. Muon and multi protons
    {
        std::set<Int_t> pdgKeys;
        pdgKeys.insert(genie::kPdgMuon);

        size_t muons = numOfParTypes(fparticleTypes,pdgKeys, false);
        pdgKeys.clear();
        pdgKeys.insert(genie::kPdgProton);
        size_t protons = numOfParTypes(fparticleTypes,pdgKeys, false);
        pdgKeys.clear();
        pdgKeys.insert(genie::kPdgMuon);
        pdgKeys.insert(genie::kPdgProton);
        size_t allExceptMuonProton = numOfParTypes(fparticleTypes,pdgKeys, true);

        if(muons == SINGLE && protons > SINGLE && allExceptMuonProton == NONE) {fEventType = EventType::MUON_AND_MULTI_PROTON_ONLY; return;}
        pdgKeys.clear();
    }
    // ======================================================

    // ======================================================
    // 6. Anti Muon and multi protons
    {
        std::set<Int_t> pdgKeys;
        pdgKeys.insert(genie::kPdgAntiMuon);

        size_t antimuons = numOfParTypes(fparticleTypes,pdgKeys, false);
        pdgKeys.clear();
        pdgKeys.insert(genie::kPdgProton);
        size_t protons = numOfParTypes(fparticleTypes,pdgKeys, false);
        pdgKeys.clear();
        pdgKeys.insert(genie::kPdgAntiMuon);
        pdgKeys.insert(genie::kPdgProton);
        size_t allExceptMuonProton = numOfParTypes(fparticleTypes,pdgKeys, true);

        if(antimuons == SINGLE && protons > SINGLE && allExceptMuonProton == NONE) {fEventType = EventType::MUON_AND_MULTI_PROTON_ONLY; return;}
        pdgKeys.clear();
    }
    // ======================================================
}
// -------------------------------------------------------------------------

}// namespace superfgd
}// namespace reconstruction
}// namespace esbroot