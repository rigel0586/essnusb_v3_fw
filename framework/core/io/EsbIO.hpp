#pragma once

#ifndef EsbGdml_hpp
#define EsbGdml_hpp 1

#include <string>

namespace esbroot {
namespace esbcore {

class EsbIO
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
    int ExportTGeoVolume(std::string path_to_file);

private:
};

}
}
#endif
