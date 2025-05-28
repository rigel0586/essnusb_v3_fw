cmake_minimum_required(VERSION 3.22)

set(Data_Target essnusb_data)

set(Data_Header ${CMAKE_CURRENT_SOURCE_DIR}/SuperFGD/FgdDetectorPoint.hpp
                           ${CMAKE_CURRENT_SOURCE_DIR}/SuperFGD/FgdHit.hpp
                           ${CMAKE_CURRENT_SOURCE_DIR}/NDCherenkov/NDCherenkovDataPoint.hpp
                           ${CMAKE_CURRENT_SOURCE_DIR}/EmulsionDetector/EmulsionDataPoint.hpp
                           ${CMAKE_CURRENT_SOURCE_DIR}/WCSim/WCSimDataPoint.hpp)

add_library(${Data_Target} SHARED)

target_sources(${Data_Target} PUBLIC 
            ${CMAKE_CURRENT_SOURCE_DIR}/SuperFGD/FgdDetectorPoint.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/SuperFGD/FgdHit.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/NDCherenkov/NDCherenkovDataPoint.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/EmulsionDetector/EmulsionDataPoint.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/WCSim/WCSimDataPoint.cpp)

target_include_directories(${Data_Target} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/SuperFGD 
                            ${CMAKE_CURRENT_SOURCE_DIR}/NDCherenkov
                            ${CMAKE_CURRENT_SOURCE_DIR}/EmulsionDetector
                            ${CMAKE_CURRENT_SOURCE_DIR}/WCSim)

LIBRARY_FOOTER(${Data_Target} ${CMAKE_CURRENT_SOURCE_DIR}/DataLinkDef.h "${Data_Header}")
