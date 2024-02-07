#pragma once

#ifndef EsbIO_hpp
#define EsbIO_hpp 1

#include <string>
#include "TObject.h"
#include <G4VPhysicalVolume.hh>

namespace esbroot {
namespace core {
namespace io {

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
    int ExportTGeoVolume(const std::string& path_to_file);

    G4VPhysicalVolume* readGdmlToGeant4(const std::string& gdml_file);

private:

    ClassDef(EsbIO, 2);
};

} // namespace io
} // namespace core
} // namespace esbroot
#endif
