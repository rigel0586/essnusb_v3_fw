set(PhysicsList_Target essnusb_physicsList)

add_library(${PhysicsList_Target} SHARED)

target_sources(${PhysicsList_Target} PUBLIC 
            ESSnusbPhysicsList.cpp)

third_party_links(${PhysicsList_Target} )

target_include_directories(${PhysicsList_Target}  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
