cmake_minimum_required(VERSION 3.22)

set(Reconstruction_Target essnusb_reconstruction)

set(Reconstruction_Header   ${CMAKE_CURRENT_SOURCE_DIR}/SuperFGD/PdgFromMomentumLoss.hpp
                            ${CMAKE_CURRENT_SOURCE_DIR}/SuperFGD/FgdTMVAEventRecord.hpp
                            ${CMAKE_CURRENT_SOURCE_DIR}/SuperFGD/FgdReconTemplate.hpp
                            ${CMAKE_CURRENT_SOURCE_DIR}/SuperFGD/FgdReconHit.hpp
                            ${CMAKE_CURRENT_SOURCE_DIR}/SuperFGD/FgdExitData.hpp
                            ${CMAKE_CURRENT_SOURCE_DIR}/SuperFGD/FgdExitParticle.hpp
                            ${CMAKE_CURRENT_SOURCE_DIR}/SuperFGD/FgdMCEventRecord.hpp
                            ${CMAKE_CURRENT_SOURCE_DIR}/SuperFGD/FgdGraphRecord.hpp
                            ${CMAKE_CURRENT_SOURCE_DIR}/SuperFGD/FgdGenFitRecon.hpp
                            ${CMAKE_CURRENT_SOURCE_DIR}/SuperFGD/FgdGraphGenFitRecon.hpp
                            ${CMAKE_CURRENT_SOURCE_DIR}/SuperFGD/FgdCalorimetric.hpp)

add_library(${Reconstruction_Target} SHARED)

target_sources(${Reconstruction_Target} PUBLIC 
            ${CMAKE_CURRENT_SOURCE_DIR}/SuperFGD/PdgFromMomentumLoss.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/SuperFGD/FgdTMVAEventRecord.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/SuperFGD/FgdReconTemplate.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/SuperFGD/FgdReconHit.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/SuperFGD/FgdExitData.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/SuperFGD/FgdMCEventRecord.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/SuperFGD/FgdExitParticle.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/SuperFGD/FgdGraphRecord.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/SuperFGD/FgdGenFitRecon.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/SuperFGD/FgdGraphGenFitRecon.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/SuperFGD/FgdCalorimetric.cpp)

target_include_directories(${Reconstruction_Target} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/SuperFGD
                                                            ${PROJECT_BASE_DIR}/geometry)

LIBRARY_FOOTER(${Reconstruction_Target} ${CMAKE_CURRENT_SOURCE_DIR}/ReconLinkDef.h "${Reconstruction_Header}")