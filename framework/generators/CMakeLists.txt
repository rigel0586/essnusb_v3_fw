cmake_minimum_required(VERSION 3.22)

set(Generators_Target essnusb_generators)

set(Generator_Header SuperFGD/GenieGenerator.h
                            SuperFGD/FgdGenieGenerator.h
                            SuperFGD/FgdGeomAnalyzer.h
                            SuperFGD/GenieFluxDriver.h)

add_library(${Generators_Target} SHARED)

target_sources(${Generators_Target} PUBLIC 
            SuperFGD/GenieGenerator.cxx
            SuperFGD/FgdGenieGenerator.cxx)

LIBRARY_FOOTER(${Generators_Target} ${Generator_Header})
