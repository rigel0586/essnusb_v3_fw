#include "EsbIO.hpp"
#include "TGeoManager.h"

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

}
}

