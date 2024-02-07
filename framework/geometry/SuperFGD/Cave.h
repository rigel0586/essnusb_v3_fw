/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *              GNU Lesser General Public Licence (LGPL) version 3,             *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#ifndef ESBROOT_GEOMETRY_Cave_H
#define ESBROOT_GEOMETRY_Cave_H

#include "TObject.h"
#include "TGeoVolume.h"

namespace esbroot {
namespace geometry {

class Cave : public TObject
{
public:
	Cave();

	virtual ~Cave();

	TGeoVolume* getVolume(){return fCaveVol;}

private:
	TGeoVolume* fCaveVol{nullptr};

	ClassDef(Cave,2)
};

} // namespace geometry
} // namespace esbroot

#endif //ESBROOT_GEOMETRY_Cave_H
