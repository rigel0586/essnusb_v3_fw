/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *              GNU Lesser General Public Licence (LGPL) version 3,             *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#include "geometry/LemmonD/SimpleLemonD.hpp"
ClassImp(esbroot::geometry::SimpleLemonD)

#include "SuperFGD/EsbSuperFGD/Materials.h"
#include "SuperFGD/EsbSuperFGD/Names.h"

#include <fairlogger/Logger.h>

#include "TGeoManager.h"
#include "TGeoMaterial.h"
#include "TGeoMedium.h"
#include "TGeoBBox.h"
#include "TGeoElement.h"

namespace esbroot {

namespace geometry {

//___________________________________________________________________
SimpleLemonD::SimpleLemonD(const std::string& outnuFile)
    : G4VSensitiveDetector("SimpleLemonD")
{
    init();
    ReadNuFluxFile(outnuFile.c_str());
}

SimpleLemonD::~SimpleLemonD()
{

}


void SimpleLemonD::ConstructGeometry()
{
    TGeoVolume *top = gGeoManager->GetTopVolume();
    if(!top)
    {
        throw "Top volume is not set!";
    }
    top->AddNode(fLemonDWater, 1, new TGeoTranslation(0, 0, 0));
}
	
bool SimpleLemonD::NextPosMomPdg(TVector3& position, TLorentzVector& momentum, int& pdgCode) 
{
    // position -> always 0 for Simple LemmonD
    position.SetX(0.);
    position.SetY(0.);
    position.SetZ(0.);

    if(fnuEntries.empty())
    {
        return false;
    }

    if(fCounter >= fnuEntries.size())
    {
        fCounter = 0;
    }

    NuFileEntry entry = fnuEntries[fCounter];

    constexpr double ENERGY_MOMENTUM_GEV = 1000; // Momentum is in MeV in file, convert to GeV
    
    double momX = entry.momX/ ENERGY_MOMENTUM_GEV;
    double momY = entry.momY/ ENERGY_MOMENTUM_GEV;
    double momZ = entry.momZ/ ENERGY_MOMENTUM_GEV;
    double E = momX*momX + momY*momY + momZ*momZ;

    momentum.SetPxPyPzE(momX, momY, momZ, E);

    pdgCode = entry.pdgNu;

    ++fCounter;
}


void SimpleLemonD::init()
{
    constexpr int lBox = 200*cm; // length in cm

    TGeoBBox* boxWater = new TGeoBBox("boxWater", lBox, lBox, lBox);

    TGeoElementTable *table = gGeoManager->GetElementTable();
    TGeoElement *hydrogen = table->GetElement(1);
    TGeoElement *oxygen = table->GetElement(8);

    TGeoMixture* matH2O = new TGeoMixture("Water", 2, 1.000);
    matH2O->AddElement(hydrogen, 2);
    matH2O->AddElement(oxygen, 1);

    TGeoMedium* waterLemonD = new TGeoMedium("waterMedium", superfgd::materials::GetNextIndex(), matH2O);
    
    fLemonDWater = new TGeoVolume(fName.c_str() ,boxWater, waterLemonD);
}

void SimpleLemonD::ReadNuFluxFile(const char* fluxFile)
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
                    int arrInd(0);
                    NuFileEntry entry;

                    std::istringstream ss(line);
                    std::string token;
                    while( (arrInd<8) && std::getline(ss, token, spaceChar))
                    {
                        if(!token.empty())
                        {
                            switch(arrInd)
                            {
                                case 0: entry.pdgNu = std::stoi(token);
                                    break;
                                case 1: entry.momX = std::stod(token);
                                    break;
                                case 2: entry.momY = std::stod(token);
                                    break;
                                case 3: entry.momZ = std::stod(token);
                                    break;
                                case 4: entry.posX = std::stod(token);
                                    break;
                                case 5: entry.posY = std::stod(token);
                                    break;
                                case 6: entry.posZ = std::stod(token);
                                    break;
                                case 7: entry.radius = std::stod(token);
                                    break;
                                default:
                                    break;
                            }
                            ++arrInd;
                        }
                    }

                    fnuEntries.emplace_back(entry);
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

} // namespace geometry
} // namespace esbroot