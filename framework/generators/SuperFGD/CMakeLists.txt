cmake_minimum_required(VERSION 3.22)

set(Generators_Target essnusb_generators)

add_library(${Generators_Target} SHARED)

target_sources(${Generators_Target} PUBLIC 
            GenieGenerator.cpp)

third_party_links(${Generators_Target})

target_include_directories(${Generators_Target} PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR})

ROOT_GENERATE_DICTIONARY( G___Generators GenieGenerator.hpp
                            EsbSuperFGD/FgdGenieGenerator.h
                            EsbSuperFGD/FgdGeomAnalyzer.h
                            EsbSuperFGD/GenieFluxDriver.h
                         LINKDEF 
                         MODULE ${Generators_Target} )
