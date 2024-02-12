cmake_minimum_required(VERSION 3.22)

set(Data_Target essnusb_data)

set(Data_Header ${CMAKE_CURRENT_SOURCE_DIR}/FgdDetectorPoint.hpp
                           ${CMAKE_CURRENT_SOURCE_DIR}/FgdHit.hpp)

add_library(${Data_Target} SHARED)

target_sources(${Data_Target} PUBLIC 
            ${CMAKE_CURRENT_SOURCE_DIR}/FgdDetectorPoint.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/FgdHit.cpp)

target_include_directories(${Data_Target} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

LIBRARY_FOOTER(${Data_Target} ${CMAKE_CURRENT_SOURCE_DIR}/DataLinkDef.h "${Data_Header}")
