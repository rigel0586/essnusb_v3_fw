cmake_minimum_required(VERSION 3.22)

set(Data_Target essnusb_data)

set(Data_Header FgdDetectorPoint.h
                            FgdHit.h)

add_library(${Data_Target} SHARED)

target_sources(${Data_Target} PUBLIC 
            FgdDetectorPoint.cxx
            FgdHit.cxx)

third_party_links(${Data_Target})

target_include_directories(${Data_Target} PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR})

ROOT_GENERATE_DICTIONARY( G___${Data_Target} ${Data_Header}
                         LINKDEF 
                         MODULE ${Data_Target} )
