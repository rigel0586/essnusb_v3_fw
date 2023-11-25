cmake_minimum_required(VERSION 3.22)

project(Essnusb C CXX)

include(./CMake/third_party.cmake)

set(ESSNUSB_PROJECT_NAME fw_essnusb)
add_executable(${ESSNUSB_PROJECT_NAME} main.cpp)

#target_link_libraries(fw_essnusb PUBLIC ROOT)
#target_link_libraries(fw_essnusb PUBLIC GTest)

add_subdirectory(core)
add_subdirectory(generators)


third_party_links(${ESSNUSB_PROJECT_NAME})

target_link_libraries(${ESSNUSB_PROJECT_NAME} PUBLIC 
                        essnusb_physicsList
                        essnusb_generators)

