set(core_Target EssCore)

add_library(${core_Target} SHARED)

target_sources(${core_Target} PUBLIC 
            ${CMAKE_CURRENT_SOURCE_DIR}/physicsList/ESSnusbPhysicsList.cpp 
            ${CMAKE_CURRENT_SOURCE_DIR}/detector/EsbDetectorConstructor.cpp 
            ${CMAKE_CURRENT_SOURCE_DIR}/io/EsbIO.cpp)

target_include_directories(${core_Target} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

#LIBRARY_FOOTER(${core_Target} ${core_headers})


third_party_links(${core_Target})
target_include_directories(${core_Target} PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR})

ROOT_GENERATE_DICTIONARY( G___${core_Target} ${CMAKE_CURRENT_SOURCE_DIR}/io/EsbIO.hpp 
                         LINKDEF ${CMAKE_CURRENT_SOURCE_DIR}/EssCoreLinkDef.h
                         MODULE ${core_Target} )





