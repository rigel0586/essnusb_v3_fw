cmake_minimum_required(VERSION 3.22)

project(Essnusb C CXX)

include(./CMake/third_party.cmake)

add_executable(fw_essnusb main.cpp)

#target_link_libraries(fw_essnusb PUBLIC ROOT)
#target_link_libraries(fw_essnusb PUBLIC GTest)

add_subdirectory(core)
add_subdirectory(generators)

target_link_libraries(fw_essnusb PUBLIC 
                        ${Geant4_LIBRARIES}
                        essnusb_physicsList
                        essnusb_generators)
