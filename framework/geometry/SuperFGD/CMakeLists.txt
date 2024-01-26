cmake_minimum_required(VERSION 3.22)

set(Geometry_Target essnusb_geometry)

set(Geometry_Header #FgdDetector.h
                            EsbSuperFGD/EsbCubeScintilatorConstructor.h
                            EsbSuperFGD/EsbFgdDetectorParameters.h
                            EsbSuperFGD/EsbSuperFGDConstructor.h
                            EsbSuperFGD/EsbSuperFGDDetectorConstruction.h
                            EsbSuperFGD/Materials.h
                            EsbSuperFGD/Names.h
                            )

add_library(${Geometry_Target} SHARED)

target_sources(${Geometry_Target} PUBLIC 
            #FgdDetector.cxx
            EsbSuperFGD/EsbCubeScintilatorConstructor.cxx
            EsbSuperFGD/EsbFgdDetectorParameters.cxx
            EsbSuperFGD/EsbSuperFGDConstructor.cxx
            EsbSuperFGD/EsbSuperFGDDetectorConstruction.cxx
            )

third_party_links(${Geometry_Target})

target_include_directories(${Geometry_Target} PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR})

ROOT_GENERATE_DICTIONARY( G___${Geometry_Target} ${Geometry_Header}
                         LINKDEF 
                         MODULE ${Geometry_Target} )
