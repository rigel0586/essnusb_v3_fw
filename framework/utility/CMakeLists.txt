cmake_minimum_required(VERSION 3.22)

set(utility_Target essnusb_utility)

add_library(${utility_Target} SHARED)

target_sources(${utility_Target} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Utility.cpp)

set(utilty_headers ${CMAKE_CURRENT_SOURCE_DIR}/Utility.hpp)

target_include_directories(${utility_Target} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/)

LIBRARY_FOOTER(${utility_Target} ${CMAKE_CURRENT_SOURCE_DIR}/UtilityLinkDef.h  "${utilty_headers}")





