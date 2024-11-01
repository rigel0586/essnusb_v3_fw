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
set(FAIRROOT_CONFIG "${FW_BASE}/fairlogger/install/lib/cmake/FairLogger-1.11.1/")
set(PATHFINDER_CONFIG "${FW_BASE}/pathfinder/install/")
set(CLHEP_CONFIG "${FW_BASE}/clhep/lib/CLHEP-2.4.6.4/")
set(VGM_CONFIG "${FW_BASE}/vgm/install/lib/VGM-5.3.0/")

set(ROOT_ROOTCLING_DIR "${FW_BASE}/cern_root/install/lib")


find_package(ROOT   REQUIRED COMPONENTS EG EGPythia6 Geom Eve RGL ROOTEve PATHS   ${ROOT_CONFIG}   NO_DEFAULT_PATH)
#add ROOT macros to have ROOT_GENERATE_DICTIONARY available
include(${ROOT_USE_FILE})
find_package(Geant4 REQUIRED COMPONENTS gdml qt ui_all vis_all PATHS   ${GEANT4_CONFIG} NO_DEFAULT_PATH)
find_package(GTest  REQUIRED PATHS   ${GOOGLE_CONFIG} NO_DEFAULT_PATH)
find_package(VGM REQUIRED PATHS ${VGM_CONFIG} NO_DEFAULT_PATH)

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
find_library (FrameAlgo libGFwAlg.so "${FW_BASE}/genie/install/lib")
#==================

#==== Genfit import
file(GLOB genfit_pcm_files "${FW_BASE}/genfit/install/lib/*.pcm")
file(GLOB genfit_rootmap_files "${FW_BASE}/genfit/install/lib/*.rootmap")
set(genfit_includes "${FW_BASE}/genfit/install/include")
set(genfit_lib "${FW_BASE}/genfit/install/lib/libgenfit2.so")
#==================

#==== Fairlogger import
find_package(FairLogger REQUIRED PATHS ${FAIRROOT_CONFIG} NO_DEFAULT_PATH)
#==================

#==== Pathfinder import
find_package(PathFinder REQUIRED PATHS ${PATHFINDER_CONFIG} NO_DEFAULT_PATH)
#==================

#==== CLHEP import
find_package(CLHEP REQUIRED PATHS ${CLHEP_CONFIG} NO_DEFAULT_PATH)
#==================
macro(third_party_links project_to_link)
    if(${ARGC} LESS 2)
         set(scope_type PUBLIC)
    else()
        set(scope_type ${ARGV1})
    endif()
    target_include_directories(${project_to_link} ${scope_type} 
                                ${log4cpp_includes} 
                                ${pythia6_includes} 
                                ${genie_includes} 
                                ${genfit_includes}
                                ${PathFinder_INCLUDE_DIR} 
                                ${CLHEP_INCLUDE_DIRS} 
                                ${Geant4_INCLUDE_DIRS} 
                                ${ROOT_INCLUDE_DIRS}
                                ${VGM_INCLUDE_DIRS})

    target_link_libraries(${project_to_link} ${scope_type} 
                                    ${genie_so_files} 
                                    ${genfit_lib}
                                    ${log4cpp_so_files} 
                                    ${Geant4_LIBRARIES} 
                                    ${ROOT_LIBRARIES} 
                                    ${VGM_LIBRARIES}
                                    ${pythia6_so_files} 
                                    FairLogger::FairLogger 
                                    ${PathFinder_LIBRARIES} 
                                    ${CLHEP_LIBRARIES}
                                    xml2 
                                    ${FrameAlgo})
endmacro()

macro(GENERATE_LIBRARY target_name rootLinkdef target_headers)
    ROOT_GENERATE_DICTIONARY( G___${target_name} "${target_headers}"
                         LINKDEF ${rootLinkdef}
                         MODULE ${target_name} )
endmacro()
    
macro(LIBRARY_FOOTER target_name rootLinkdef)
    third_party_links(${target_name})

    # Remove previous .pcm module files. This is done because once created the module is not deleted. Thus implementations are not updated
    # The .pcm will be recreated when .C script is invoked with the wanted class from that library
    get_filename_component(PCM_FILE ${ROOT_ROOTCLING_DIR}/${target_name}.pcm ABSOLUTE)
   
    if(EXISTS "${PCM_FILE}")
            message(WARNING " Deleteing previous pcm file at: ${PCM_FILE} ")
            file(REMOVE ${PCM_FILE})
    endif()

    GENERATE_LIBRARY(${target_name} "${rootLinkdef}" "${ARGN}")
endmacro()

macro(GENERATE_LIBRARY_WITH_DEPENDENCY target_name rootLinkdef dependencyModules target_headers)
    ROOT_GENERATE_DICTIONARY( G___${target_name} "${target_headers}"
                         LINKDEF ${rootLinkdef} 
                         MODULE ${target_name} DEPENDENCIES ${dependencyModules})
endmacro()
    
macro(LIBRARY_FOOTER_WITH_DEPENDENCY target_name rootLinkdef dependencyModules)
    third_party_links(${target_name})

    # Remove previous .pcm module files. This is done because once created the module is not deleted. Thus implementations are not updated
    # The .pcm will be recreated when .C script is invoked with the wanted class from that library
    get_filename_component(PCM_FILE ${ROOT_ROOTCLING_DIR}/${target_name}.pcm ABSOLUTE)
   
    if(EXISTS "${PCM_FILE}")
            message(WARNING " Deleteing previous pcm file at: ${PCM_FILE} ")
            file(REMOVE ${PCM_FILE})
    endif()

    GENERATE_LIBRARY_WITH_DEPENDENCY(${target_name} "${rootLinkdef}" "${dependencyModules}" "${ARGN}")
endmacro()

macro(UT_libs target_name)
    target_link_libraries(${target_name} PUBLIC ROOT)
    target_link_libraries(${target_name} PUBLIC GTest)
endmacro()

