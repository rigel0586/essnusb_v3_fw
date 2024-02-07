/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *              GNU Lesser General Public Licence (LGPL) version 3,             *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#include "geometry/SuperFGD/Cave.h"
ClassImp(esbroot::geometry::Cave)

#include "SuperFGD/EsbSuperFGD/Materials.h"
#include "SuperFGD/EsbSuperFGD/Names.h"

#include "TGeoManager.h"
#include "TGeoMaterial.h"
#include "TGeoMedium.h"
#include "TGeoBBox.h"
#include "TGeoElement.h"

namespace esbroot {

namespace geometry {

//___________________________________________________________________
Cave::Cave()
{
    int lBox = 20000; // length in cm

    TGeoBBox* caveBox = new TGeoBBox("caveBox", lBox, lBox, lBox);

    // air                3  14.01  16.  39.95  7.  8.  18.  1.205e-3  .755  .231  .014
    TGeoElementTable *table = gGeoManager->GetElementTable();
    TGeoMixture *airMix = new TGeoMixture(esbroot::geometry::superfgd::materials::air,3, 1.205e-3);
    TGeoElement *nitrogen = table->GetElement(7);
    TGeoElement *oxygen = table->GetElement(8);
    TGeoElement *argon = table->GetElement(18);
    airMix->AddElement(nitrogen, 0.755);
    airMix->AddElement(oxygen, 0.231);
    airMix->AddElement(argon, 0.014);
    TGeoMedium* air = new TGeoMedium("airCave", superfgd::materials::GetNextIndex(), airMix);
    
    fCaveVol = new TGeoVolume("caveBox",caveBox, air);
}

} // namespace geometry
} // namespace esbroot