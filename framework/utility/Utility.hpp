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
  
  void findVolume(const std::string& searchName
                    , G4VPhysicalVolume* topVolume
                    , std::vector<G4VPhysicalVolume*>& foundVols
                    , VolumeSearchType searchtype );
  
  private:
  

  ClassDef(Utility, 2);
};
 
}
}

#endif // ESBROOT_ESBDATA_SUPERFGD_FGD_HIT_H
