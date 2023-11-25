cmake_minimum_required(VERSION 3.22)

project(Essnusb C CXX)

include(./CMake/third_party.cmake)

set(ESSNUSB_PROJECT_NAME fw_essnusb)
add_executable(${ESSNUSB_PROJECT_NAME} main.cpp)

#target_link_libraries(fw_essnusb PUBLIC ROOT)
#target_link_libraries(fw_essnusb PUBLIC GTest)

add_subdirectory(core)
add_subdirectory(generators)

target_include_directories(${ESSNUSB_PROJECT_NAME} PUBLIC
                        ${log4cpp_includes}
                        ${pythia6_includes}
                        ${genie_includes})

target_link_libraries(${ESSNUSB_PROJECT_NAME} PUBLIC 
                        essnusb_physicsList
                        essnusb_generators
                        ${log4cpp_so_files}
                        ${pythia6_so_files}
                        ${genie_so_files})
