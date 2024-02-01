cmake_minimum_required(VERSION 3.22)

set(Geometry_Target essnusb_geometry)

set(Geometry_Header SuperFGD/FgdDetector.h
                            SuperFGD/EsbSuperFGD/EsbCubeScintilatorConstructor.h
                            SuperFGD/EsbSuperFGD/EsbFgdDetectorParameters.h
                            SuperFGD/EsbSuperFGD/EsbSuperFGDConstructor.h
                            SuperFGD/EsbSuperFGD/EsbSuperFGDDetectorConstruction.h
                            SuperFGD/EsbSuperFGD/Materials.h
                            SuperFGD/EsbSuperFGD/Names.h
                            )

add_library(${Geometry_Target} SHARED)

target_sources(${Geometry_Target} PUBLIC 
            SuperFGD/FgdDetector.cxx
            SuperFGD/EsbSuperFGD/EsbCubeScintilatorConstructor.cxx
            SuperFGD/EsbSuperFGD/EsbFgdDetectorParameters.cxx
            SuperFGD/EsbSuperFGD/EsbSuperFGDConstructor.cxx
            SuperFGD/EsbSuperFGD/EsbSuperFGDDetectorConstruction.cxx
            )

LIBRARY_FOOTER(${Geometry_Target} ${Geometry_Header})
