#include "EsbIO.hpp"
ClassImp(esbroot::core::io::EsbIO)

#include "TGeoManager.h"
#include "G4GDMLParser.hh"

namespace esbroot {
namespace core {
namespace io {


EsbIO::EsbIO()
{
}

EsbIO::~EsbIO()
{
}

int EsbIO::ExportTGeoVolume(const std::string& path_to_file){
    return gGeoManager->Export(path_to_file.c_str());
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

} // namespace io
} // namespace core
} // namespace esbroot
