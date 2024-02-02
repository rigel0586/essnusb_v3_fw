#include "EsbTest.hpp"
#include "TGeoManager.h"

int EsbTest::getInt()
{
    return 42;
}

int EsbTest::toGdml(std::string path_to_file){
        return gGeoManager->Export(path_to_file.c_str());
}


