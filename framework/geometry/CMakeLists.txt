cmake_minimum_required(VERSION 3.22)

set(Geometry_Target essnusb_geometry)

set(Geometry_Header ${CMAKE_CURRENT_SOURCE_DIR}/SuperFGD/FgdDetector.h
                            ${CMAKE_CURRENT_SOURCE_DIR}/SuperFGD/EsbSuperFGD/EsbCubeScintilatorConstructor.h
                            ${CMAKE_CURRENT_SOURCE_DIR}/SuperFGD/EsbSuperFGD/EsbFgdDetectorParameters.h
                            ${CMAKE_CURRENT_SOURCE_DIR}/SuperFGD/EsbSuperFGD/EsbSuperFGDConstructor.h
                            ${CMAKE_CURRENT_SOURCE_DIR}/SuperFGD/EsbSuperFGD/EsbSuperFGDDetectorConstruction.h
                            ${CMAKE_CURRENT_SOURCE_DIR}/SuperFGD/EsbSuperFGD/Materials.h
                            ${CMAKE_CURRENT_SOURCE_DIR}/SuperFGD/EsbSuperFGD/Names.h
                            )

add_library(${Geometry_Target} SHARED)

target_sources(${Geometry_Target} PUBLIC 
            ${CMAKE_CURRENT_SOURCE_DIR}/SuperFGD/FgdDetector.cxx
            ${CMAKE_CURRENT_SOURCE_DIR}/SuperFGD/EsbSuperFGD/EsbCubeScintilatorConstructor.cxx
            ${CMAKE_CURRENT_SOURCE_DIR}/SuperFGD/EsbSuperFGD/EsbFgdDetectorParameters.cxx
            ${CMAKE_CURRENT_SOURCE_DIR}/SuperFGD/EsbSuperFGD/EsbSuperFGDConstructor.cxx
            ${CMAKE_CURRENT_SOURCE_DIR}/SuperFGD/EsbSuperFGD/EsbSuperFGDDetectorConstruction.cxx
            )

LIBRARY_FOOTER(${Geometry_Target} ${Geometry_Header})
