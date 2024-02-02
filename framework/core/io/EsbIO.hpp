#pragma once

#ifndef EsbIO_hpp
#define EsbIO_hpp 1

#include <string>
//#include "G4VPhysicalVolume.hh"
#include "TObject.h"

//namespace io {

class EsbIO : public TObject
{
public:
    EsbIO();
    ~EsbIO();

    /**
      @brief Exports to out file using TGeoManager->Export
      @param[in]  path_to_file to export the Top volume to
        ///  - Case 1: root file or root/xml file
        ///    if filename end with ".root". The key will be named name
        ///    By default the geometry is saved without the voxelisation info.
        ///    Use option 'v" to save the voxelisation info.
        ///    if filename end with ".xml" a root/xml file is produced.
        ///
        ///  - Case 2: C++ script
        ///    if filename end with ".C"
        ///
        ///  - Case 3: gdml file
        ///    if filename end with ".gdml"
        ///    NOTE that to use this option, the PYTHONPATH must be defined like
        ///      export PYTHONPATH=$ROOTSYS/lib:$ROOTSYS/geom/gdml
    */
    static int ExportTGeoVolume(std::string path_to_file);

    //static G4VPhysicalVolume* readGdmlToGeant4(std::string gdml_file);

    ClassDef(EsbIO,2);
};

//}
#endif
