cmake_minimum_required(VERSION 3.22)

set(Generators_Target essnusb_generators)

add_library(${Generators_Target} SHARED)

target_sources(${Generators_Target} PUBLIC 
            GenieGenerator.cpp)

target_include_directories(${Generators_Target} PUBLIC 
                                ${CMAKE_CURRENT_SOURCE_DIR}
                                ${log4cpp_includes}
                                ${pythia6_includes}
                                ${genie_includes}
                                ${ROOT_INCLUDE_DIRS})

target_link_libraries(${Generators_Target} PUBLIC
                                xml2
                                ${genie_so_files}
                                ${log4cpp_so_files}
                                ${Geant4_LIBRARIES}
                                ${ROOT_LIBRARIES}
                                ${pythia6_so_files})
