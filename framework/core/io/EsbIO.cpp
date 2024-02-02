#include "EsbIO.hpp"
#include "TGeoManager.h"
#include "G4GDMLParser.hh"


namespace esbroot {
namespace esbcore {

EsbIO::EsbIO()
{
}

EsbIO::~EsbIO()
{
}

int EsbIO::ExportTGeoVolume(std::string path_to_file){
    return gGeoManager->Export(path_to_file.c_str());
}

G4VPhysicalVolume* EsbIO::readGdmlToGeant4(std::string gdml_file)
{
    G4GDMLParser parser;
    parser.SetOverlapCheck(true);
    parser.Read(gdml_file.c_str());
    return parser.GetWorldVolume();
}

}
}

