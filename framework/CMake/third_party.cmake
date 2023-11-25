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


find_package(ROOT   REQUIRED COMPONENTS EG EGPythia6 PATHS   ${ROOT_CONFIG}   NO_DEFAULT_PATH)
find_package(Geant4 REQUIRED PATHS   ${GEANT4_CONFIG} NO_DEFAULT_PATH)
find_package(GTest  REQUIRED PATHS   ${GOOGLE_CONFIG} NO_DEFAULT_PATH)

#==== Genie dependencies import
file(GLOB log4cpp_so_files "${FW_BASE}/log4cpp/lib/*.so")
file(GLOB log4cpp_a_files "${FW_BASE}/log4cpp/lib/*.a")
set(log4cpp_includes "${FW_BASE}/log4cpp/include")


file(GLOB pythia6_so_files "${FW_BASE}/pythia6/lib/*.so")
file(GLOB pythia6_a_files "${FW_BASE}/pythia6/lib/*.a")
set(pythia6_includes "${FW_BASE}/pythia6/inc")
#==================

#==== Genie import
file(GLOB genie_so_files "${FW_BASE}/genie/install/lib/*.so")
set(genie_includes "${FW_BASE}/genie/install/include/GENIE")
#==================

#==== Genfit import
file(GLOB genfit_pcm_files "${FW_BASE}/genfit/install/lib/*.pcm")
file(GLOB genfit_rootmap_files "${FW_BASE}/genfit/install/lib/*.rootmap")
set(genfit_includes "${FW_BASE}/genfit/install/include")
#==================
