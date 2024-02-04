set(core_Target EssCore)

add_library(${core_Target} SHARED)

target_sources(${core_Target} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/io/EsbIO.cpp
            )

set(core_headers ${CMAKE_CURRENT_SOURCE_DIR}/io/EsbIO.hpp 
                    )

target_include_directories(${core_Target} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/
                                                    ${CMAKE_CURRENT_SOURCE_DIR}/io)

LIBRARY_FOOTER(${core_Target} ${CMAKE_CURRENT_SOURCE_DIR}/EssCoreLinkDef.h  "${core_headers}")





