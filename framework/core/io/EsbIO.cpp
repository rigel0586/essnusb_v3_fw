#include "EsbIO.hpp"
ClassImp(esbroot::core::io::EsbIO)

#include "TGeoManager.h"
#include "G4GDMLParser.hh"
#include "Geant4GM/volumes/Factory.h"
#include "RootGM/volumes/Factory.h"
#include <fairlogger/Logger.h>
#include <filesystem>

namespace esbroot {
namespace core {
namespace io {


EsbIO::EsbIO()
{
    fg4Parser = new G4GDMLParser();
}

EsbIO::~EsbIO()
{
}

int EsbIO::ExportTGeoVolume(const std::string& path_to_file){
    return gGeoManager->Export(path_to_file.c_str());
}

int EsbIO::ExportG4Volume(const std::string& path_to_file, G4VPhysicalVolume* g4Volume){
    if(fg4Parser == nullptr) return -1;

    fg4Parser->Write(path_to_file, g4Volume);
}

void EsbIO::ExportG4VolumeVGM(const std::string& path_to_file, G4VPhysicalVolume* g4Volume)
{
    // Import Geant4 geometry to VGM
    Geant4GM::Factory g4Factory;
    g4Factory.SetDebug(1);
    g4Factory.Import(g4Volume);
    // 
    // Export VGM geometry to Root
    RootGM::Factory rtFactory;
    rtFactory.SetDebug(1);
    g4Factory.Export(&rtFactory);
    gGeoManager->CloseGeometry();
    gGeoManager->Export(path_to_file.c_str());
}

void EsbIO::ConvertGDMLToRoorVGM(const std::string& in_path_to_gdml_file, const std::string& out_path_to_root_file)
{
    G4GDMLParser parser;
    parser.SetOverlapCheck(true);
    parser.Read(in_path_to_gdml_file.c_str(), false);
    G4VPhysicalVolume* g4Volume = parser.GetWorldVolume();

    ExportG4VolumeVGM(out_path_to_root_file, g4Volume);
}

bool EsbIO::ImportTGeoVolume(const std::string& path_to_file){
    auto geoMng = gGeoManager->Import(path_to_file.c_str());
    return (geoMng != nullptr);
}

G4VPhysicalVolume* EsbIO::readGdmlToGeant4(const std::string& gdml_file)
{
    G4GDMLParser parser;
    parser.SetOverlapCheck(true);
    parser.Read(gdml_file.c_str(), false);
    return parser.GetWorldVolume();
}

void EsbIO::printG4Volume(G4VPhysicalVolume* g4vol, int tabs)
{
    G4LogicalVolume* lv =	g4vol->GetLogicalVolume();
    G4int limit = lv->GetNoDaughters();
    for(int i = 0; i < limit; ++i)
    {
        G4VPhysicalVolume * daug = lv->GetDaughter(i);
        for(int t = 0; t < tabs; ++t){
            std::cout << "\t";
        }
        std::cout << daug->GetName() << std::endl;
        printG4Volume(daug, ++tabs);
    }
}

bool EsbIO::deleteFile(const std::string& file_Path)
{
    std::filesystem::path fPath_remove{file_Path};
    if(std::filesystem::exists(fPath_remove)){
        bool isDeleted = std::filesystem::remove(fPath_remove);
        LOG(info) << "Deleting: " << fPath_remove << " : [" << (isDeleted ? "true" : "false") << "]";
        return isDeleted;
    }

    LOG(info) << "File: " << fPath_remove << " not found to be deleted. ";
    return false;
}

} // namespace io
} // namespace core
} // namespace esbroot
