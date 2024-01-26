#ifndef ESBROOT_ESBDRECONSTRUCTION_FGD_MC_EVENT_RECORDT_H
#define ESBROOT_ESBDRECONSTRUCTION_FGD_MC_EVENT_RECORDT_H

#include <TObject.h>
#include <TVector3.h>

#include <vector>
#include <ostream>
#include <map>
#include <set>

namespace esbroot {
namespace reconstruction {
namespace superfgd {

class FgdMCEventRecord : public TObject
{
public:

    enum EventType : int   // Position in the fDataTokens which value represents which data 
    {
        MUON_ONLY,
        ANTI_MUON_ONLY,

        MUON_AND_PROTON_ONLY,
        ANTI_MUON_AND_PROTON_ONLY,

        MUON_AND_MULTI_PROTON_ONLY,
        ANTI_MUON_AND_MULTI_PROTON_ONLY,

        UNDEFINED
    };

    FgdMCEventRecord(std::string eventData);

    FgdMCEventRecord();

    /* Copy constructor */
    FgdMCEventRecord(const FgdMCEventRecord& c);

    virtual ~FgdMCEventRecord();

    /* Operators */
    FgdMCEventRecord& operator=(const FgdMCEventRecord& c);

    void SetEventData(std::string data);

    Int_t GetNuPdg(void);
    Double_t GetNuE(void);
    TVector3 GetVertex(void);
    Bool_t IsWeakCC(void);
    Bool_t IsWeakNC(void);
    Bool_t IsQuasiElastic(void);

    Bool_t IsDeepInElastic(void);
    Bool_t IsResonant(void);
    Bool_t IsCoherentProduction(void);
    Bool_t IsMEC(void);
    Bool_t IsNuElectronElastic(void);
    Bool_t IsElectronScattering(void);


    const std::vector<std::pair<Int_t, TVector3>>& GetPrimaryParticles();

    Bool_t HasHits(void);
    void SetHasHits(Bool_t h){fHasHits = h;}

    Bool_t IsPrimaryLeptonMuon();
    Bool_t IsMuonExiting(){return fIsMuonExiting;}
    void SetMuonExiting(Bool_t muonExit) {fIsMuonExiting = muonExit;}
    TVector3 GetMuonExitMom() {return fMuonExitMomentum;}
    void SetMuonExitMom(TVector3 exitMom);
    Double_t GetHadronEdep() {return fHadronEdep;}
    void SetHadronEdep(Double_t edep) {fHadronEdep = edep;}
    
    Double_t GetMuonPolarAngle();
    Double_t GetMuonAzumuteAngle();

    TVector3 GetMuonMom();
    Double_t SetMuonTrackLength(Double_t ml) {fMuonTrackLength = ml;}
    Double_t GetMuonTrackLength();
    Double_t SetMuonTrackLengthOrigin(Double_t ml) {fMuonTrackLengthOrigin = ml;}
    Double_t GetMuonTrackLengthOrigin();
    TVector3 GetGenfitMom(){return fGenfitMom;}
    void SetGenfitMom(TVector3 genfitMom){fGenfitMom = genfitMom;}
    void SetMomError(Double_t err){fMC_GentFitError = err;}
    Double_t GetMomError(){return fMC_GentFitError;}

    void SetProtonEdep(Double_t e){fProtonEdep = e;} // in [MeV]
    Double_t GetProtonEdep(){return fProtonEdep;}

    Bool_t IsPrimaryLeptonElectron();
    TVector3 GetElectronMom();
    Int_t GetNumOfExitingPar(){return fElectronNumOfExitingParticles;}
    void SetNumOfExitingPar(Int_t numPar){fElectronNumOfExitingParticles = numPar;}
    const std::vector<Int_t>  GetPdgOfExitingPars(){return fElectronExitingPdg;}
    void  SetPdgOfExitingPars(const std::vector<Int_t> pars){fElectronExitingPdg = pars;}

    void SetDeDx(Double_t e) { fDeDx = e;}
    Double_t GetDeDx() {return fDeDx;}

    void SetEdepDeDx(Double_t e) { fEdepDeDx = e;}
    Double_t GetEdepDeDx() {return fEdepDeDx;}

    std::string GetEventData(){return feventData; }
    EventType GetEventType() {return fEventType; }

    void PrintData(std::ostream & stream);
    void ReadEventData();

protected:

    enum Data : int   // Position in the fDataTokens which value represents which data 
    {
        NEUTRINO_PDG = 0,
        NEUTRINO_ENERGY = 1,
        IS_WEAK_CC = 2,
        IS_WEACK_NC = 3,
        IS_QUASI_ELASTIC = 4,

        IS_DEEP_INELASRIC = 5,
        IS_RESONANT = 6,
        IS_COHERENT_PRODUCTION = 7,
        IS_MEC = 8,
        IS_NuElectron_Elastic = 9,
        IS_ELECTRON_SCATTERING = 10,

        VERTEX_POSTION_X = 11,
        VERTEX_POSTION_Y = 12,
        VERTEX_POSTION_Z = 13,
        PRIMARY_PARTICLES = 14   // The primary particles are written as 4 values - 1st pdg then momentum X, momentum Y, momentum Z
                                // all data above 14th position are primary particle data
    };

    void Init();
    void InitMembers();
    void fillType();
    size_t numOfParTypes(std::map<Int_t, Int_t>& map, std::set<Int_t>& keys, bool countAllExceptKeys);
    void determineEventType();
    std::string feventData;

    
    Int_t fnuPdg;
    Double_t fNuEnergy;
    TVector3 fvertex;
    Bool_t fIsWeakCC;
    Bool_t fIsWeakNC;
    Bool_t fIsQuasiElastic;

    Bool_t fIsDeepInElastic;
    Bool_t fIsResonant;
    Bool_t fIsCoherentProduction;
    Bool_t fIsMEC;
    Bool_t fIsNuElectronElastic;
    Bool_t fIsElectronScattering;

    Bool_t fHasHits;

    Bool_t fIsPrimaryMuon;
    Bool_t fIsMuonExiting;
    TVector3 fMuonExitMomentum;
    Double_t fMuonPolarAngle;
    Double_t fMuonAzumAngle;
    Double_t fHadronEdep;
    TVector3 fGenfitMom;
    Double_t fMC_GentFitError;

    Bool_t fIsPrimaryElectron;
    std::vector<Int_t> fElectronExitingPdg;//!<!
    Int_t fElectronNumOfExitingParticles;

    TVector3 fPrimaryMuonMom;
    TVector3 fPrimaryElectronMom;

    Double_t fMuonTrackLength;
    Double_t fMuonTrackLengthOrigin;

    Double_t fProtonEdep;
    Double_t fDeDx;
    Double_t fEdepDeDx;
    
    std::vector<std::string> fDataTokens;
    std::vector<std::pair<Int_t, TVector3>> fPrimaryParticles;//!<!

    EventType fEventType;//!<!
    std::map<Int_t, Int_t> fparticleTypes;//!<!

    ClassDef(FgdMCEventRecord, 1);

};

} //superfgd
} //reconstruction
} //esbroot

#endif // ESBROOT_ESBDRECONSTRUCTION_FGD_MC_GENFIT_H