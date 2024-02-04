cmake_minimum_required(VERSION 3.22)

project(Essnusb C CXX)

include(./CMake/third_party.cmake)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(ESSNUSB_PROJECT_NAME fw_essnusb)

#message(WARNING ${ROOT_CXX_FLAGS})

add_subdirectory(core)
add_subdirectory(data)
add_subdirectory(generators)
add_subdirectory(geometry)


