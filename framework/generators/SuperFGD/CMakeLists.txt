cmake_minimum_required(VERSION 3.22)

set(Generators_Target essnusb_generators)

set(Generator_Header GenieGenerator.hpp
                            EsbSuperFGD/FgdGenieGenerator.h
                            EsbSuperFGD/FgdGeomAnalyzer.h
                            EsbSuperFGD/GenieFluxDriver.h)

add_library(${Generators_Target} SHARED)

target_sources(${Generators_Target} PUBLIC 
            GenieGenerator.cpp)

third_party_links(${Generators_Target})

target_include_directories(${Generators_Target} PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR})

ROOT_GENERATE_DICTIONARY( G___${Generators_Target} ${Generator_Header}
                         LINKDEF 
                         MODULE ${Generators_Target} )
