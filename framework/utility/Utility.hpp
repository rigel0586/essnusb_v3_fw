/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *              GNU Lesser General Public Licence (LGPL) version 3,             *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/


#ifndef ESBROOT_ESBUTILITY_H
#define ESBROOT_ESBUTILITY_H

#include <vector>
#include <string>

#include <TObject.h>

#include "CLHEP/Units/SystemOfUnits.h"

#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"

namespace esbroot {
namespace utility {

enum class VolumeSearchType{
    MatchName,
    Contains
};

class Utility : public TObject
{
 public:
  
    Utility();
    ~Utility() = default;
  
    void findVolume(const std::string& searchName
                      , G4VPhysicalVolume* topVolume
                      , std::vector<G4VPhysicalVolume*>& foundVols
                      , VolumeSearchType searchtype );

    double rootToG4CoeffLength();
    double rootToG4CoeffEnergy();
    
  private:
  
    constexpr static double ROOT_LENGTH = 1; // By default root inteprets length in [cm]
    constexpr static double G4_LENGTH = 1 * CLHEP::m; // Geant uses 1 for [mm]

    constexpr static double ROOT_E = 1; // We interpret Energy in GeV
    constexpr static double G4_E = 1 * CLHEP::GeV; 

  ClassDef(Utility, 2);
};
 
}
}

#endif // ESBROOT_ESBDATA_SUPERFGD_FGD_HIT_H
