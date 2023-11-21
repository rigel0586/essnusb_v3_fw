cmake_minimum_required(VERSION 3.22)

set(Generators_Target essnusb_generators)

add_library(${Generators_Target} SHARED)

target_sources(${Generators_Target} PUBLIC 
            GenieGenerator.cpp)

target_include_directories(${Generators_Target} PUBLIC 
                                ${CMAKE_CURRENT_SOURCE_DIR}
                                ${ROOT_INCLUDE_DIRS})

target_link_libraries(${Generators_Target} 
                                fw_genie 
                                ${ROOT_LIBRARIES})
