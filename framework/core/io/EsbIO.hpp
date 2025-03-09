#pragma once

#ifndef EsbIO_hpp
#define EsbIO_hpp 1

#include <string>
#include "TObject.h"
#include <G4VPhysicalVolume.hh>

#include "G4GDMLParser.hh"
#include "G4VPhysicalVolume.hh"

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

    /**
      @brief Import to out file using TGeoManager->Import
      @param[in]  path_to_file to import the volume to TGeoManager
        /// Case 1: gdml if filename ends with ".gdml" the foreign geometry described with gdml 
        /// is imported executing some python scripts in $ROOTSYS/gdml. NOTE that to use this option, 
        /// the PYTHONPATH must be defined like export PYTHONPATH=$ROOTSYS/lib:$ROOTSYS/gdml
        ///
        /// Case 2: root file (.root) or root/xml file (.xml) Import in memory from filename 
        /// the geometry with key=name. if name="" (default), the first TGeoManager object in the file is returned.
    */
    bool ImportTGeoVolume(const std::string& path_to_file);

    /**
      @brief Exports to out file using G4GDMLParser->Write
      @param[in]  path_to_file to export the Top geant4 volume to
      @param[in]  g4Volume g4 volume to export
    */
    int ExportG4Volume(const std::string& path_to_file, G4VPhysicalVolume* g4Volume);

    /**
      @brief Exports to out file using VGM
      @param[in]  path_to_file to export the Top geant4 volume to (will be converted to root file)
      @param[in]  path_to_file g4 volume to export
    */
    void ExportG4VolumeVGM(const std::string& path_to_file, G4VPhysicalVolume* g4Volume);


    /**
      @brief Read gdml file and Exports it to out root file using VGM
      @param[in]  in_path_to_gdml_file  Path to gdml file to read g4 volume
      @param[in]  out_path_to_root_file path to export to root format)
    */
    void ConvertGDMLToRoorVGM(const std::string& in_path_to_gdml_file, const std::string& out_path_to_root_file);

    G4VPhysicalVolume* readGdmlToGeant4(const std::string& gdml_file);

    void printG4Volume(G4VPhysicalVolume* g4vol, int tabs = 1);

    bool deleteFile(const std::string& file_Path);

private:

    ClassDef(EsbIO, 2);
};

} // namespace io
} // namespace core
} // namespace esbroot
#endif
