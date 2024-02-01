#pragma once

#ifndef EsbIDetector_hpp
#define EsbIDetector_hpp 1

#include "TGeoVolume.h"
#include <string>

class IDetector
{
    public:
        virtual ~IDetector() = default;

        virtual TGeoVolume* getVolume() = 0;
        virtual void getPosition(double& posX, double& posY, double& posZ) = 0;
        virtual std::string& getName() = 0;
};


#endif
