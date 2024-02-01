#pragma once

#ifndef EsbIDetector_hpp
#define EsbIDetector_hpp 1

#include "TGeoVolume.h"
#include <string>

namespace esbroot {
namespace esbcore {

class IDetector
{
    public:
        virtual ~IDetector() = default;

        virtual void ConstructGeometry() = 0;
};

}
}

#endif
