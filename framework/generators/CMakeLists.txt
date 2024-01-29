cmake_minimum_required(VERSION 3.22)

set(Generators_Target essnusb_generators)

set(Generator_Header SuperFGD/GenieGenerator.hpp
                            SuperFGD/EsbSuperFGD/FgdGenieGenerator.h
                            SuperFGD/EsbSuperFGD/FgdGeomAnalyzer.h
                            SuperFGD/EsbSuperFGD/GenieFluxDriver.h)

add_library(${Generators_Target} SHARED)

target_sources(${Generators_Target} PUBLIC 
            SuperFGD/GenieGenerator.cpp)

LIBRARY_FOOTER(${Generators_Target} ${Generator_Header})
