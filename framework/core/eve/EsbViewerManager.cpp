#include "EsbViewerManager.hpp"
ClassImp(esbroot::core::eve::EsbViewerManager)

#include "TGeoManager.h"
#include <fairlogger/Logger.h>

namespace esbroot {
namespace core {
namespace eve {

EsbViewerManager::EsbViewerManager(const std::string& inputGeomFile) 
    : finputGeomFile(inputGeomFile)
{
}

EsbViewerManager::~EsbViewerManager()
{
}

void EsbViewerManager::visualize()
{
    gGeoManager->Import(finputGeomFile.c_str());
    gGeoManager->GetTopVolume()->Draw("ogl");
}

} // namespace eve
} // namespace core
} // namespace esbroot
