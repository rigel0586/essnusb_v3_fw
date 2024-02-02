cmake_minimum_required(VERSION 3.22)

set(Data_Target essnusb_data)

set(Data_Header ${CMAKE_CURRENT_SOURCE_DIR}/FgdDetectorPoint.h
                           ${CMAKE_CURRENT_SOURCE_DIR}/FgdHit.h)

add_library(${Data_Target} SHARED)

target_sources(${Data_Target} PUBLIC 
            ${CMAKE_CURRENT_SOURCE_DIR}/FgdDetectorPoint.cxx
            ${CMAKE_CURRENT_SOURCE_DIR}/FgdHit.cxx)

LIBRARY_FOOTER(${Data_Target} ${Data_Header})
