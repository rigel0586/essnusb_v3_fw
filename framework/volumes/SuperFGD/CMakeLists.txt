cmake_minimum_required(VERSION 3.22)

set(Volumes_Target essnusb_volumes)

set(Volumes_Header #FgdDetector.h
                            EsbSuperFGD/EsbCubeScintilatorConstructor.h
                            EsbSuperFGD/EsbFgdDetectorParameters.h
                            EsbSuperFGD/EsbSuperFGDConstructor.h
                            EsbSuperFGD/EsbSuperFGDDetectorConstruction.h
                            EsbSuperFGD/Materials.h
                            EsbSuperFGD/Names.h
                            )

add_library(${Volumes_Target} SHARED)

target_sources(${Volumes_Target} PUBLIC 
            #FgdDetector.cxx
            EsbSuperFGD/EsbCubeScintilatorConstructor.cxx
            EsbSuperFGD/EsbFgdDetectorParameters.cxx
            EsbSuperFGD/EsbSuperFGDConstructor.cxx
            EsbSuperFGD/EsbSuperFGDDetectorConstruction.cxx
            )

third_party_links(${Volumes_Target})

target_include_directories(${Volumes_Target} PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR})

ROOT_GENERATE_DICTIONARY( G___${Volumes_Target} ${Volumes_Header}
                         LINKDEF 
                         MODULE ${Volumes_Target} )
