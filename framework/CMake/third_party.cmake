if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 11.2.0)
   message(FATAL_ERROR "${CMAKE_CXX_COMPILER_VERSION} g++ version must be at least 11.3.0!")
endif()

if (CMAKE_C_COMPILER_VERSION VERSION_LESS 11.2.0)
   message(FATAL_ERROR "${CMAKE_C_COMPILER_VERSION} GCC version must be at least 11.3.0!")
endif()

set(FW_BASE ${CMAKE_CURRENT_SOURCE_DIR}/..)
set(ROOT_CONFIG "${FW_BASE}/cern_root/install/cmake/")
set(GEANT4_CONFIG "${FW_BASE}/geant4/install/lib/cmake/Geant4/")
set(GOOGLE_CONFIG "${FW_BASE}/googletest/install/lib/cmake/GTest/")


find_package(ROOT   REQUIRED PATHS   ${ROOT_CONFIG}   NO_DEFAULT_PATH)
find_package(Geant4 REQUIRED PATHS   ${GEANT4_CONFIG} NO_DEFAULT_PATH)
find_package(GTest  REQUIRED PATHS   ${GOOGLE_CONFIG} NO_DEFAULT_PATH)

#==== Genie import
file(GLOB genie_so_files "${FW_BASE}/genie/install/lib/*.so")
add_library(fw_genie INTERFACE)
target_link_libraries(fw_genie INTERFACE ${genie_so_files})
target_include_directories(fw_genie INTERFACE "${FW_BASE}/genie/install/include")
#==================

#==== Genfit import
file(GLOB genfit_pcm_files "${FW_BASE}/genfit/install/lib/*.pcm")
file(GLOB genfit_rootmap_files "${FW_BASE}/genfit/install/lib/*.rootmap")
add_library(fw_genfit INTERFACE)
set_target_properties(fw_genfit
                      PROPERTIES genfit_pcm ${genfit_pcm_files}
                      genfit_rootmap ${genfit_rootmap_files})
target_link_libraries(fw_genfit INTERFACE "${FW_BASE}/genfit/install/lib/libgenfit2.so")
target_include_directories(fw_genfit INTERFACE "${FW_BASE}/genfit/install/include")
#==================
