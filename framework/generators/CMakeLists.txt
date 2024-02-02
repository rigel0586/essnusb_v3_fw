cmake_minimum_required(VERSION 3.22)

set(Generators_Target essnusb_generators)

set(Generator_Header ${CMAKE_CURRENT_SOURCE_DIR}/SuperFGD/GenieGenerator.h
                            ${CMAKE_CURRENT_SOURCE_DIR}/SuperFGD/FgdGenieGenerator.h
                            ${CMAKE_CURRENT_SOURCE_DIR}/SuperFGD/FgdGeomAnalyzer.h
                            ${CMAKE_CURRENT_SOURCE_DIR}/SuperFGD/GenieFluxDriver.h)

add_library(${Generators_Target} SHARED)

target_sources(${Generators_Target} PUBLIC 
            ${CMAKE_CURRENT_SOURCE_DIR}/SuperFGD/GenieGenerator.cxx
            ${CMAKE_CURRENT_SOURCE_DIR}/SuperFGD/FgdGenieGenerator.cxx)

LIBRARY_FOOTER(${Generators_Target} ${Generator_Header})
