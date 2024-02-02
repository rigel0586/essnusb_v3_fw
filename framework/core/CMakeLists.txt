set(core_Target essnusb_root_core)

add_library(${core_Target} SHARED)

set(core_headers ${CMAKE_CURRENT_SOURCE_DIR}/io/EsbIO.hpp 
            ${CMAKE_CURRENT_SOURCE_DIR}/detector/IDetector.hpp 
            ${CMAKE_CURRENT_SOURCE_DIR}/detector/EsbDetectorConstructor.hpp 
            #${CMAKE_CURRENT_SOURCE_DIR}/hit/EsbG4Hit.h 
            ${CMAKE_CURRENT_SOURCE_DIR}/physicsList/ESSnusbPhysicsList.hpp )

target_sources(${core_Target} PUBLIC 
            ${CMAKE_CURRENT_SOURCE_DIR}/physicsList/ESSnusbPhysicsList.cpp 
            ${CMAKE_CURRENT_SOURCE_DIR}/detector/EsbDetectorConstructor.cpp 
            ${CMAKE_CURRENT_SOURCE_DIR}/io/EsbIO.cpp)

target_include_directories(${core_Target} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/io)

LIBRARY_FOOTER(${core_Target} ${core_headers})
