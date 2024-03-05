cmake_minimum_required(VERSION 3.22)

project(Essnusb C CXX)

include(./CMake/third_party.cmake)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(PROJECT_BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(ESSNUSB_PROJECT_NAME fw_essnusb)

add_subdirectory(core)
add_subdirectory(data)
add_subdirectory(digitizer)
add_subdirectory(eve)
add_subdirectory(generators)
add_subdirectory(geometry)
add_subdirectory(reconstruction)
add_subdirectory(utility)

set(fw_path ${CMAKE_CURRENT_SOURCE_DIR})
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/main.in ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp @ONLY)

# Compile stand alone application for openGL qt visualization
set(EsbSimApp esb_sim_app)
set(EsbSim_Deps EsbCore
                essnusb_data
                essnusb_digitizer
                essnusb_eve
                essnusb_generators
                essnusb_geometry
                essnusb_reconstruction
                essnusb_utility)

add_executable(${EsbSimApp} ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

target_include_directories(${EsbSimApp} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_dependencies(${EsbSimApp} ${EsbSim_Deps})

target_link_libraries(${EsbSimApp} PUBLIC ${EsbSim_Deps})
third_party_links(${EsbSimApp})





