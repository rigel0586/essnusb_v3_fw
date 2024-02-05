set(core_Target EsbCore)

add_library(${core_Target} SHARED)

target_sources(${core_Target} PUBLIC 
                ${CMAKE_CURRENT_SOURCE_DIR}/io/EsbIO.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/physicsList/ESSnusbPhysicsList.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/detector/EsbDetectorConstructor.cpp)

set(core_headers ${CMAKE_CURRENT_SOURCE_DIR}/io/EsbIO.hpp 
                 ${CMAKE_CURRENT_SOURCE_DIR}/physicsList/ESSnusbPhysicsList.hpp 
                 ${CMAKE_CURRENT_SOURCE_DIR}/detector/EsbDetectorConstructor.hpp)

target_include_directories(${core_Target} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/
                                                    ${CMAKE_CURRENT_SOURCE_DIR}/io
                                                    ${CMAKE_CURRENT_SOURCE_DIR}/physicsList
                                                    ${CMAKE_CURRENT_SOURCE_DIR}/detector)

LIBRARY_FOOTER(${core_Target} ${CMAKE_CURRENT_SOURCE_DIR}/EssCoreLinkDef.h  "${core_headers}")





