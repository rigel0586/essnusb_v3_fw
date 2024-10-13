cmake_minimum_required(VERSION 3.22)

set(Generators_Target essnusb_generators)

set(Generator_Header        ${CMAKE_CURRENT_SOURCE_DIR}/SuperFGD/GenieGenerator.h
                            ${CMAKE_CURRENT_SOURCE_DIR}/SuperFGD/FgdGenieGenerator.h
                            ${CMAKE_CURRENT_SOURCE_DIR}/SuperFGD/FgdGeomAnalyzer.h
                            ${CMAKE_CURRENT_SOURCE_DIR}/SuperFGD/GenieFluxDriver.h
                            ${CMAKE_CURRENT_SOURCE_DIR}/generic/GenieGenerator.h
                            ${CMAKE_CURRENT_SOURCE_DIR}/generic/GenericGenieGenerator.h
                            ${CMAKE_CURRENT_SOURCE_DIR}/generic/GenericGeomAnalyzer.h
                            ${CMAKE_CURRENT_SOURCE_DIR}/generic/GenieFluxDriver.h
                            ${CMAKE_CURRENT_SOURCE_DIR}/generic/IFluxNextPosition.h
                            ${CMAKE_CURRENT_SOURCE_DIR}/NDCherenkov/NDSimpleGenerator.h)

add_library(${Generators_Target} SHARED)

target_include_directories(${Generators_Target} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_sources(${Generators_Target} PUBLIC 
            ${CMAKE_CURRENT_SOURCE_DIR}/SuperFGD/GenieGenerator.cxx
            ${CMAKE_CURRENT_SOURCE_DIR}/SuperFGD/FgdGenieGenerator.cxx
            ${CMAKE_CURRENT_SOURCE_DIR}/SuperFGD/FgdGeomAnalyzer.cxx
            ${CMAKE_CURRENT_SOURCE_DIR}/SuperFGD/GenieFluxDriver.cxx
            ${CMAKE_CURRENT_SOURCE_DIR}/generic/GenieGenerator.cxx
            ${CMAKE_CURRENT_SOURCE_DIR}/generic/GenericGenieGenerator.cxx
            ${CMAKE_CURRENT_SOURCE_DIR}/generic/GenericGeomAnalyzer.cxx
            ${CMAKE_CURRENT_SOURCE_DIR}/generic/GenieFluxDriver.cxx
            ${CMAKE_CURRENT_SOURCE_DIR}/NDCherenkov/NDSimpleGenerator.cpp)


LIBRARY_FOOTER(${Generators_Target} ${CMAKE_CURRENT_SOURCE_DIR}/GeneratorsLinkDef.h  "${Generator_Header}")
