cmake_minimum_required(VERSION 3.22)

set(Digitizer_Target essnusb_digitizer)

set(Digitizer_Header ${CMAKE_CURRENT_SOURCE_DIR}/SuperFGD/FgdDigitizer.hpp
                           ${CMAKE_CURRENT_SOURCE_DIR}/SuperFGD/FgdMppcDisplay.hpp)

add_library(${Digitizer_Target} SHARED)

target_sources(${Digitizer_Target} PUBLIC 
            ${CMAKE_CURRENT_SOURCE_DIR}/SuperFGD/FgdDigitizer.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/SuperFGD/FgdMppcDisplay.cpp)

target_include_directories(${Digitizer_Target} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/SuperFGD)

LIBRARY_FOOTER(${Digitizer_Target} ${CMAKE_CURRENT_SOURCE_DIR}/DigitizerLinkDef.h "${Digitizer_Header}")
