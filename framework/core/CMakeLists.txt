set(core_Target essnusb_root_core)

add_library(${core_Target} SHARED)

set(core_headers io/EsbIO.hpp 
            detector/IDetector.hpp 
            detector/EsbDetectorConstructor.hpp 
            #hit/EsbG4Hit.h 
            physicsList/ESSnusbPhysicsList.hpp )

target_sources(${core_Target} PUBLIC 
            physicsList/ESSnusbPhysicsList.cpp 
            detector/EsbDetectorConstructor.cpp 
            io/EsbIO.cpp)

target_include_directories(${core_Target} PUBLIC io)

LIBRARY_FOOTER(${core_Target} ${core_headers})
