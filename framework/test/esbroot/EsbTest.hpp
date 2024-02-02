#pragma once

#include <string>
#include "TObject.h"


class EsbTest : public TObject{
public:
    int getInt();

    int toGdml(std::string path_to_file);

    ClassDef(EsbTest,2);
};


