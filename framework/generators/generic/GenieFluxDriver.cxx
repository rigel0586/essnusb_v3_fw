#include "generators/generic/GenieFluxDriver.h"
ClassImp(esbroot::generators::generic::GenieFluxDriver)

#include "Framework/ParticleData/PDGCodes.h"
#include "Framework/Conventions/Units.h"

#include <fairlogger/Logger.h>

#include <iostream>

using namespace std;

namespace esbroot {
namespace generators {
namespace generic {

GenieFluxDriver::GenieFluxDriver(const char* nuFluxFile
                            , IFluxNextPosition* ifluxposition
                            , unsigned int seed
                            , Bool_t uniformFlux
                            , Int_t maxEvents
                            , Double_t maxEnergy)
    :   fnuFluXFile(nuFluxFile)
        , fFluxNextPosition(ifluxposition)
        , fType(FluxDriverType::Basic)
        , f_generator_Id(0)
        , frndGen(seed)
        , frdnGenDeault(seed)
        , fdis(0.0, 1.0)
        , fpdgCode(0)
        , fMaxEv(maxEnergy)
        , fcurrentEvent(0)
        , fmaxEvents(maxEvents)
        , fUniformFlux(uniformFlux)
{ 
    InitPDGList();
    Init4Momentum();
    Init4Position();

    ReadNuFluxFile(fnuFluXFile.c_str());
    CalculateProbability();
}


GenieFluxDriver::GenieFluxDriver(const char* nuFluxFile
                            , std::vector<IFluxNextPosition*> ifluxpositions
                            , unsigned int seed
                            , Bool_t uniformFlux
                            , Int_t maxEvents
                            , Double_t maxEnergy)
    :   fnuFluXFile(nuFluxFile)
        , fFluxNextPositions(ifluxpositions)
        , fType(FluxDriverType::Composite)
        , f_generator_Id(0)
        , frndGen(seed)
        , frdnGenDeault(seed)
        , fdis(0.0, 1.0)
        , fpdgCode(0)
        , fMaxEv(maxEnergy)
        , fcurrentEvent(0)
        , fmaxEvents(maxEvents)
        , fUniformFlux(uniformFlux)
{ 
    InitPDGList();
    Init4Momentum();
    Init4Position();

    ReadNuFluxFile(fnuFluXFile.c_str());
    CalculateProbability();
}

GenieFluxDriver::GenieFluxDriver(const GenieFluxDriver& gf)
{
    this->fnuFluXFile = gf.fnuFluXFile;
    this->fFluxNextPosition = gf.fFluxNextPosition;
    this->fFluxNextPositions = gf.fFluxNextPositions;
    this->f_generator_Id = gf.f_generator_Id;
    this->frndGen = gf.frndGen;
    this->fdis = gf.fdis;
    
    this->fMaxEv = gf.fMaxEv;
    this->fpdgCode = gf.fpdgCode;
    this->fcurrentEvent = gf.fcurrentEvent;

    this->fPdgCList = gf.fPdgCList;
    this->f4momentum = gf.f4momentum;
    this->f4position = gf.f4position;

    this->f_total_X = gf.f_total_X;
    this->f_total_Y = gf.f_total_Y;
    this->f_total_Z = gf.f_total_Z;
    this->fnuFluXFile = gf.fnuFluXFile;
    this->fFlux = gf.fFlux;
    this->fmaxEvents = gf.fmaxEvents;

    this-> fUniformFlux = gf.fUniformFlux;
}

GenieFluxDriver& GenieFluxDriver::operator=(const GenieFluxDriver& gf)
{
    this->fnuFluXFile = gf.fnuFluXFile;
    this->fFluxNextPosition = gf.fFluxNextPosition;
    this->fFluxNextPositions = gf.fFluxNextPositions;
    this->f_generator_Id = gf.f_generator_Id;
    this->frndGen = gf.frndGen;
    this->fdis = gf.fdis;

    this->fMaxEv = gf.fMaxEv;
    this->fpdgCode = gf.fpdgCode;
    this->fcurrentEvent = gf.fcurrentEvent;

    this->fPdgCList = gf.fPdgCList;
    this->f4momentum = gf.f4momentum;
    this->f4position = gf.f4position;

    this->f_total_X = gf.f_total_X;
    this->f_total_Y = gf.f_total_Y;
    this->f_total_Z = gf.f_total_Z;
    this->fnuFluXFile = gf.fnuFluXFile;
    this->fFlux = gf.fFlux;
    this->fmaxEvents = gf.fmaxEvents;
    this-> fUniformFlux = gf.fUniformFlux;

    return *this;
}

bool GenieFluxDriver::GenerateNext(void)
{
    static size_t uniformId = 0;
    if(fUniformFlux && !fFlux.empty())
    {
        GenieFluxDriver::FLuxNeutrino& neutrino = fFlux[uniformId];
        fpdgCode = neutrino.GetPdg();
        CalculateNext4position();
        CalculateNext4Momentum(neutrino.GetEnergy());
        fcurrentEvent++;

        uniformId++;
        if(uniformId>=fFlux.size())
            uniformId = 0;

        return true;
    }

    int nuPdg(0);
    Double_t nuEnergy(0.);
    Double_t nextNu = fdis(frndGen);
    //Double_t nextNu = fdis(frdnGenDeault);

    for(size_t i = 0; i < fFlux.size(); ++i)
    {
        GenieFluxDriver::FLuxNeutrino& neutrino = fFlux[i];
        
        if(neutrino.GetNeutrino(nextNu, nuPdg, nuEnergy))
        {
            CalculateNext4position();
            CalculateNext4Momentum(nuEnergy);
            fpdgCode = nuPdg;
            
            fcurrentEvent++;

            return true;
        }        
    }

    return false;
}

//-------------------------------------------------------
//                  Private methods
//-------------------------------------------------------

// Calculate the starting position of the neutrino
void GenieFluxDriver::CalculateNext4position()
{
    // static std::uniform_real_distribution<Double_t> ldis(-0.5,0.5);
    // Double_t x_det = f_total_X * ldis(frndGen);
    // Double_t y_det = f_total_Y * ldis(frndGen);
    // Double_t z_det = f_total_Z * ldis(frndGen);

    if(fType == FluxDriverType::Basic && fFluxNextPosition == nullptr){
        LOG(fatal) <<  "IFluxNextPosition is not set! Exiting ...";
        exit(0);
    }

    if(fType == FluxDriverType::Composite && fFluxNextPositions.empty()){
        LOG(fatal) <<  "IFluxNextPosition positions is not set! Exiting ...";
        exit(0);
    }

    TVector3 pos_det;
    
    if(fType == FluxDriverType::Basic){
        pos_det  = fFluxNextPosition->NextVertexPosition();
    }

    if(fType == FluxDriverType::Composite){
        int id = f_generator_Id % fFluxNextPositions.size();
        pos_det  = fFluxNextPositions[id]->NextVertexPosition();
    }

    // Set the Position of the event
    Double_t rndm_X = pos_det.X();
    Double_t rndm_Y = pos_det.Y();
    Double_t rndm_Z = pos_det.Z();

    // Double_t rndm_X = pos_det.X();
    // Double_t rndm_Y = pos_det.Y();
    // Double_t rndm_Z = pos_det.Z();

    f4position.SetX(rndm_X);
    f4position.SetY(rndm_Y);
    // Used if rootgeaomanalyzer GenerateVertex
    // since it gives the starting position of neutrino in Z direction
    // and computes where it interacts along the way. Otherwise give out coordinates
    f4position.SetZ(rndm_Z); 
    // f4position.SetZ(rndm_Z);

    // f4position.SetX(x_det);
    // f4position.SetY(y_det);
    // f4position.SetZ(z_det);

    f4position.SetT(0.);

    ++f_generator_Id;
}

void GenieFluxDriver::CalculateNext4Momentum(Double_t energyOfNeutrino)
{
    // calculate only pZ since beam is parallel to Z axis
    // neutrino is massless so p=E/c
    Double_t pZ = energyOfNeutrino;///CLHEP::c_light;
    Double_t pX = 0;
    Double_t pY = 0;

    f4momentum.SetPxPyPzE(pX,pY,pZ,energyOfNeutrino);
}

void GenieFluxDriver::InitPDGList(void)
{
    /* The neutrinos are added in the list in the order in which they are expected to appear in the flux file*/

    // Matter neutrinos
    fPdgCList.push_back(genie::kPdgNuE);        // Electron neutrino pdgCode = 12
    fPdgCList.push_back(genie::kPdgNuMu);       // Muon neutrino pdgCode = 14
    fPdgCList.push_back(genie::kPdgNuTau);      // Electron tau pdgCode = 16


    // Antimatter neutrinos
    fPdgCList.push_back(genie::kPdgAntiNuE);    // Electron antineutrino pdgCode = -12
    fPdgCList.push_back(genie::kPdgAntiNuMu);   // Muon antineutrino pdgCode = -14
    fPdgCList.push_back(genie::kPdgAntiNuTau);  // Tau antineutrino pdgCode = -16
}

void GenieFluxDriver::Init4Momentum(void)
{
    f4momentum.SetX(0.);
    f4momentum.SetY(0.);
    f4momentum.SetZ(0.);
    f4momentum.SetT(0);
}

void GenieFluxDriver::Init4Position(void)
{
    f4position.SetX(0.);
    f4position.SetY(0.);
    f4position.SetZ(0.);
    f4position.SetT(0);
}

void GenieFluxDriver::ReadNuFluxFile(const char* fluxFile)
{
    std::string nufluxFile(fluxFile);

    if(!nufluxFile.empty())
    {
        const char spaceChar(' ');
        std::ifstream fluxFileStream;

        try
        {
            fluxFileStream.open(nufluxFile.c_str(), std::ios::in);

            if(fluxFileStream.is_open())
            {
                std::string line;
                while(std::getline(fluxFileStream,line))
                {
                    Double_t arr[] = {0,0,0,0,0,0,0};
                    int arrInd(0);

                    std::istringstream ss(line);
                    std::string token;
                    while( (arrInd<7) && std::getline(ss, token, spaceChar))
                    {
                        if(!token.empty())
                        {
                            Double_t parsedVal = std::stod(token);
                            arr[arrInd++] = parsedVal;
                        }
                    }

                    /* The first value is the energy value for all neutrinos on the line */
                    Double_t energy = arr[0];

                    /* The neutrinos are added in the list in the order in which they are expected to appear in the flux file*/
                    for(size_t i = 1; i < 7; ++i)
                    {
                        if(arr[i] != 0)
                        {
                            fFlux.emplace_back(fPdgCList[i-1], energy, arr[i]);
                        }
                    }
                }
            }
        }
        catch(const std::exception& e)
        {
            LOG(fatal) << e.what();
        }

        if(fluxFileStream.is_open())
        {
            fluxFileStream.close();
        }
    }
}

void GenieFluxDriver::CalculateProbability()
{
    Double_t totalFlux(0.);
    for(size_t i = 0; i < fFlux.size(); ++i)
    {
        GenieFluxDriver::FLuxNeutrino* neutrino = &fFlux[i];
        totalFlux+= neutrino->GetFlux();
    }

    Double_t currentLowerFluxVal(0.);
    for(size_t i = 0; i < fFlux.size(); ++i)
    {
        GenieFluxDriver::FLuxNeutrino& neutrino = fFlux[i];
        neutrino.SetLowerVal(currentLowerFluxVal);
        Double_t uppVal = currentLowerFluxVal + neutrino.GetFlux()/totalFlux;
        neutrino.SetUpperVal(uppVal);
        currentLowerFluxVal = uppVal;
    }
}


GenieFluxDriver::FLuxNeutrino::FLuxNeutrino(int pdg , Double_t energy, Double_t flux)
    : fpdg(pdg), fEnergy(energy), fFluxValue(flux), fLowerVal(0.) , fUpperVal(0.)
{
}

GenieFluxDriver::FLuxNeutrino::~FLuxNeutrino()
{
}

GenieFluxDriver::FLuxNeutrino::FLuxNeutrino(const FLuxNeutrino& fn)
{
    fpdg = fn.fpdg;
    fEnergy = fn.fEnergy;
    fFluxValue = fn.fFluxValue;

    fLowerVal = fn.fLowerVal;
    fUpperVal = fn.fUpperVal;
}

GenieFluxDriver::FLuxNeutrino& GenieFluxDriver::FLuxNeutrino::operator=(const FLuxNeutrino& fn)
{
    fpdg = fn.fpdg;
    fEnergy = fn.fEnergy;
    fFluxValue = fn.fFluxValue;

    fLowerVal = fn.fLowerVal;
    fUpperVal = fn.fUpperVal;

    return *this;
}

Bool_t GenieFluxDriver::FLuxNeutrino::GetNeutrino(Double_t val, int& pdg, Double_t& energy)
{
    if(val>=fLowerVal && val <fUpperVal)
    {
        pdg = fpdg;
        energy = fEnergy;
        return true;
    }
    return false;
}

} //namespace generic 
} //namespace generators
} //namespace esbroot
