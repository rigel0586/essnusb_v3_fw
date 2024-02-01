set(core_Target essnusb_root_core)

add_library(${core_Target} SHARED)

set(core_headers 
            detector/IDetector.hpp
            detector/EsbDetectorConstructor.hpp
            hit/EsbG4Hit.h
            physicsList/ESSnusbPhysicsList.hpp
            io/EsbIO.hpp)

target_sources(${core_Target} PUBLIC 
            physicsList/ESSnusbPhysicsList.cpp
            detector/EsbDetectorConstructor.cpp
            io/EsbIO.cpp)

LIBRARY_FOOTER(${core_Target} ${core_headers})
