/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *              GNU Lesser General Public Licence (LGPL) version 3,             *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#include "utility/Utility.hpp"
ClassImp(esbroot::utility::Utility)

namespace esbroot {
namespace utility {

// -----   Default constructor   -------------------------------------------
Utility::Utility()
{
}
// -------------------------------------------------------------------------


 
void Utility::findVolume(const std::string& searchName
                    , G4VPhysicalVolume* topVolume
                    , std::vector<G4VPhysicalVolume*>& foundVols
                    , VolumeSearchType searchtype )
{
    G4LogicalVolume* lv =	topVolume->GetLogicalVolume();
    G4int limit = lv->GetNoDaughters();
    for(int i = 0; i < limit; ++i)
    {
        G4VPhysicalVolume * ph_daug = lv->GetDaughter(i);
        G4LogicalVolume * daug = ph_daug->GetLogicalVolume();
        switch(searchtype){
            case VolumeSearchType::MatchName :  if(daug->GetName() == searchName) foundVols.emplace_back(ph_daug);
                                                break;
            case VolumeSearchType::Contains  :  if(daug->GetName().find(searchName) != std::string::npos) foundVols.emplace_back(ph_daug);
                                                break;
            case VolumeSearchType::Excludes  :  if(daug->GetName().find(searchName) == std::string::npos) foundVols.emplace_back(ph_daug);
                                                break;
            default:
                                                break;
        }
        findVolume(searchName, ph_daug, foundVols, searchtype);
    }
}

double Utility::rootToG4CoeffLength()
{
    // return 1;
    return (G4_LENGTH/ROOT_LENGTH);
}

double Utility::rootToG4CoeffEnergy()
{
    // return 1;
    return (G4_E/ROOT_E);
}

double Utility::g4ToRootCoeffLength()
{
    return (1/rootToG4CoeffLength());
}

double Utility::g4ToRootCoeffEnergy()
{
    return (1/rootToG4CoeffEnergy());
}


}
}
