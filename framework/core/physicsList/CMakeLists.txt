set(PhysicsList_Target essnusb_physicsList)

add_library(${PhysicsList_Target} INTERFACE)

target_sources(${PhysicsList_Target} INTERFACE 
            ESSnusbPhysicsList.cpp)

target_include_directories(${PhysicsList_Target}  INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
