set(core_Target Test)

add_library(${core_Target} SHARED)

set(core_headers ${CMAKE_CURRENT_SOURCE_DIR}/esbroot/EsbTest.hpp )

target_sources(${core_Target} PUBLIC 
            ${CMAKE_CURRENT_SOURCE_DIR}/esbroot/EsbTest.cpp)

target_include_directories(${core_Target} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

third_party_links(${core_Target})
target_include_directories(${core_Target} PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR})

ROOT_GENERATE_DICTIONARY( G___${core_Target} ${CMAKE_CURRENT_SOURCE_DIR}/esbroot/EsbTest.hpp
                         LINKDEF 
                         MODULE ${core_Target} )
