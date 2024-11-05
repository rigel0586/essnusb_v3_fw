#pragma once

#ifndef EsbViewerManager_hpp
#define EsbViewerManager_hpp 1

#include <string>

#include "TObject.h"

namespace esbroot {
namespace core {
namespace eve {

class EsbViewerManager : public TObject
{
public:
    EsbViewerManager(const std::string& inputGeomFile);
    ~EsbViewerManager();

    void visualize();
private:
    const std::string finputGeomFile;

    ClassDef(EsbViewerManager, 2);
};

} // namespace eve
} // namespace core
} // namespace esbroot
#endif
