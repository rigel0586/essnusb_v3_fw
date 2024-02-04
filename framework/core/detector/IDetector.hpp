#pragma once

#ifndef EsbIDetector_hpp
#define EsbIDetector_hpp 1

#include "TGeoVolume.h"
#include "TObject.h"
#include <string>

namespace esbroot {
namespace core {
namespace detector {

class IDetector : public TObject
{
    public:
        virtual ~IDetector() = default;

        virtual void ConstructGeometry() = 0;

private:

    ClassDef(IDetector, 2);
};

} // namespace detector
} // namespace core
} // namespace esbroot

#endif
