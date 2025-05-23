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
                            ${CMAKE_CURRENT_SOURCE_DIR}/NDCherenkov/PhysicsList/NeutronHPphysics.hh
                            ${CMAKE_CURRENT_SOURCE_DIR}/NDCherenkov/NDCherenkov.hpp
                            ${CMAKE_CURRENT_SOURCE_DIR}/EmulsionDetector/EmulsionDetector.hpp
                            ${CMAKE_CURRENT_SOURCE_DIR}/EmulsionDetector/DetectorDefinition.hpp
                            ${CMAKE_CURRENT_SOURCE_DIR}/EmulsionDetector/MagneticField.hpp
                            ${CMAKE_CURRENT_SOURCE_DIR}/WCSim/WCSimGeometry.hpp
                            ${CMAKE_CURRENT_SOURCE_DIR}/WCSim/WCSimPmtInfo.hpp
                            ${CMAKE_CURRENT_SOURCE_DIR}/WCSim/WCSimPMTObject.hpp
                            ${CMAKE_CURRENT_SOURCE_DIR}/WCSim/WCSimRootOptions.hpp
                            ${CMAKE_CURRENT_SOURCE_DIR}/WCSim/WCSimEnumerations.hpp
                            ${CMAKE_CURRENT_SOURCE_DIR}/WCSim/WCSimWCPMT.hpp
                            ${CMAKE_CURRENT_SOURCE_DIR}/WCSim/WCSimWCHit.hpp
                            ${CMAKE_CURRENT_SOURCE_DIR}/WCSim/WCSimWCDigi.hpp
                            ${CMAKE_CURRENT_SOURCE_DIR}/WCSim/WCSimTuningParameters.hpp
                            ${CMAKE_CURRENT_SOURCE_DIR}/WCSim/WCSimLC.hpp)

add_library(${Geometry_Target} SHARED)

target_sources(${Geometry_Target} PUBLIC 
            ${CMAKE_CURRENT_SOURCE_DIR}/SuperFGD/EsbSuperFGD/CubeScintConstructor.cxx
            ${CMAKE_CURRENT_SOURCE_DIR}/SuperFGD/EsbSuperFGD/FgdDetectorParameters.cxx
            ${CMAKE_CURRENT_SOURCE_DIR}/SuperFGD/EsbSuperFGD/SuperFGDConstructor.cxx
            ${CMAKE_CURRENT_SOURCE_DIR}/SuperFGD/EsbSuperFGD/SuperFGDDetectorConstruction.cxx
            ${CMAKE_CURRENT_SOURCE_DIR}/SuperFGD/FgdDetector.cxx
            ${CMAKE_CURRENT_SOURCE_DIR}/SuperFGD/Cave.cxx
            ${CMAKE_CURRENT_SOURCE_DIR}/NDCherenkov/PhysicsList/NeutronHPphysics.cc
            ${CMAKE_CURRENT_SOURCE_DIR}/NDCherenkov/NDCherenkov.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/EmulsionDetector/EmulsionDetector.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/EmulsionDetector/DetectorDefinition.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/EmulsionDetector/MagneticField.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/WCSim/WCSimGeometry.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/WCSim/WCSimConstructMaterials.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/WCSim/WCSimPmtInfo.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/WCSim/WCSimPMTObject.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/WCSim/WCSimRootOptions.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/WCSim/WCSimEnumerations.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/WCSim/WCSimWCPMT.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/WCSim/WCSimWCHit.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/WCSim/WCSimWCDigi.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/WCSim/WCSimTuningParameters.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/WCSim/WCSimDetectorConfigs.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/WCSim/WCSimConstructPMT.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/WCSim/WCSimLC.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/WCSim/WCSimConstructEggShapedHyperK.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/WCSim/WCSimConstructCylinder.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/WCSim/WCSimConstructGeometryTables.cpp)

target_include_directories(${Geometry_Target} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
                                                ${CMAKE_CURRENT_SOURCE_DIR}/SuperFGD
                                                ${CMAKE_CURRENT_SOURCE_DIR}/SuperFGD/EsbSuperFGD
                                                ${CMAKE_CURRENT_SOURCE_DIR}/NDCherenkov/
                                                ${CMAKE_CURRENT_SOURCE_DIR}/NDCherenkov/PhysicsList
                                                ${CMAKE_CURRENT_SOURCE_DIR}/WCSim
)

LIBRARY_FOOTER(${Geometry_Target} ${CMAKE_CURRENT_SOURCE_DIR}/GeometryLinkDef.h "${Geometry_Header}")

