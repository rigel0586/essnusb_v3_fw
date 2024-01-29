#add_subdirectory(actions)
#add_subdirectory(detectorConstruction)
#add_subdirectory(physicsList)

set(core_Target essnusb_root_core)

add_library(${core_Target} SHARED)

set(core_headers 
            physicsList/ESSnusbPhysicsList.hpp
            hit/EsbCoreHit.h)

target_sources(${core_Target} PUBLIC 
            physicsList/ESSnusbPhysicsList.cpp)

LIBRARY_FOOTER(${core_Target} ${core_headers})
