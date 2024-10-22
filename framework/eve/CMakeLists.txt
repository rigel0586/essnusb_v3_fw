cmake_minimum_required(VERSION 3.22)

set(eve_Target essnusb_eve)

set(eve_Header ${CMAKE_CURRENT_SOURCE_DIR}/SuperFGD/EveEvent.hpp
               ${CMAKE_CURRENT_SOURCE_DIR}/NdCherenkov/EveEvent.hpp
               ${CMAKE_CURRENT_SOURCE_DIR}/EmulsionDetector/EveEvent.hpp)

add_library(${eve_Target} SHARED)

target_sources(${eve_Target} PUBLIC 
            ${CMAKE_CURRENT_SOURCE_DIR}/SuperFGD/EveEvent.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/NdCherenkov/EveEvent.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/EmulsionDetector/EveEvent.cpp)

target_include_directories(${eve_Target} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/SuperFGD)

LIBRARY_FOOTER(${eve_Target} ${CMAKE_CURRENT_SOURCE_DIR}/EveLinkDef.h "${eve_Header}")
