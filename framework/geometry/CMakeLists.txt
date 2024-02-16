cmake_minimum_required(VERSION 3.22)

set(Geometry_Target essnusb_geometry)

set(Geometry_Header ${CMAKE_CURRENT_SOURCE_DIR}/SuperFGD/FgdDetector.h
                            ${CMAKE_CURRENT_SOURCE_DIR}/SuperFGD/Cave.h
                            ${CMAKE_CURRENT_SOURCE_DIR}/SuperFGD/EsbSuperFGD/CubeScintConstructor.h
                            ${CMAKE_CURRENT_SOURCE_DIR}/SuperFGD/EsbSuperFGD/FgdDetectorParameters.h
                            ${CMAKE_CURRENT_SOURCE_DIR}/SuperFGD/EsbSuperFGD/SuperFGDConstructor.h
                            ${CMAKE_CURRENT_SOURCE_DIR}/SuperFGD/EsbSuperFGD/SuperFGDDetectorConstruction.h
                            ${CMAKE_CURRENT_SOURCE_DIR}/SuperFGD/EsbSuperFGD/Materials.h
                            ${CMAKE_CURRENT_SOURCE_DIR}/SuperFGD/EsbSuperFGD/Names.h
                            )

add_library(${Geometry_Target} SHARED)

target_sources(${Geometry_Target} PUBLIC 
            ${CMAKE_CURRENT_SOURCE_DIR}/SuperFGD/EsbSuperFGD/CubeScintConstructor.cxx
            ${CMAKE_CURRENT_SOURCE_DIR}/SuperFGD/EsbSuperFGD/FgdDetectorParameters.cxx
            ${CMAKE_CURRENT_SOURCE_DIR}/SuperFGD/EsbSuperFGD/SuperFGDConstructor.cxx
            ${CMAKE_CURRENT_SOURCE_DIR}/SuperFGD/EsbSuperFGD/SuperFGDDetectorConstruction.cxx
            ${CMAKE_CURRENT_SOURCE_DIR}/SuperFGD/FgdDetector.cxx
            ${CMAKE_CURRENT_SOURCE_DIR}/SuperFGD/Cave.cxx)

target_include_directories(${Geometry_Target} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
                                                ${CMAKE_CURRENT_SOURCE_DIR}/SuperFGD
                                                ${CMAKE_CURRENT_SOURCE_DIR}/SuperFGD/EsbSuperFGD
)

LIBRARY_FOOTER(${Geometry_Target} ${CMAKE_CURRENT_SOURCE_DIR}/GeometryLinkDef.h "${Geometry_Header}")

