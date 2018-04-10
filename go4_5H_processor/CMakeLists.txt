# ===============================================

# https://root.cern.ch/how/integrate-root-my-project-cmake

cmake_minimum_required(VERSION 3.5)

project(go4_5H_processor)

set(CMAKE_VERBOSE_MAKEFILE ON)

# ===============================================
# External packages

# ROOT
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
find_package(ROOT REQUIRED)

#---Define useful ROOT functions and macros (e.g. ROOT_GENERATE_DICTIONARY)
include(${ROOT_USE_FILE})

# Go4
if (DEFINED ENV{GO4SYS})
    message("Go4 found at $ENV{GO4SYS}")
    include_directories($ENV{GO4SYS}/include)
else()
    message(FATAL_ERROR "Go4 not found! Please, run go4login or define the GO4SYS env. variable.")
endif()

# # ExpertRoot
# if (DEFINED ENV{ERSYS})
#     message("ExpertRoot found at $ENV{ERSYS}")
#     include_directories($ENV{ERSYS})
# else()
#     message(FATAL_ERROR "ExpertRoot not found! Please, define the ERSYS env. variable pointing to the ExpertRoot source directory.")
# endif()

# # ExpertRoot build
# if (DEFINED ENV{ERBUILD})
#     message("ExpertRoot build found at $ENV{ERBUILD}")
# else()
#     message(FATAL_ERROR "ExpertRoot build not found! Please, define the ERBUILD env. variable pointing to the ExpertRoot build directory.")
# endif()

# # FairRoot
# if (DEFINED ENV{FAIRROOTPATH})
#     message("FairRoot found at $ENV{FAIRROOTPATH}")
#     include_directories($ENV{FAIRROOTPATH}/include)
# else()
#     message(FATAL_ERROR "FairRoot not found! Please, run [er_build_dir]/config.sh or define the FAIRROOTPATH env. variable.")
# endif()

# # FairSoft
# if (DEFINED ENV{SIMPATH})
#     message("FairSoft found at $ENV{SIMPATH}")
#     include_directories($ENV{SIMPATH}/include)
# else()
#     message(FATAL_ERROR "FairSoft not found! Please, run [er_build_dir]/config.sh or define the SIMPATH env. variable.")
# endif()

# ===============================================

# if (CMAKE_COMPILER_IS_GNUCXX)
#     set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wextra -Wpedantic -std=c++11")
# endif()

# ===============================================
# ROOT dictionary

include_directories(${CMAKE_SOURCE_DIR})

set(ROOTCLING_HEADERS UserAnalysis.h
                      UserEvent.h
                      UserProc.h
                      UserEventStep2.h
                      UserProcStep2.h
                      UserAnalysisHistos.h
                      data/BeamDetMWPCDigi.h
                      )

ROOT_GENERATE_DICTIONARY(G__UserAnalysis ${ROOTCLING_HEADERS} LINKDEF UserAnalysisLinkDef.h)

# ===============================================

set(SOURCE_LIB G__UserAnalysis.cxx
               UserAnalysis.cxx
               UserEvent.cxx
               UserProc.cxx
               UserEventStep2.cxx
               UserProcStep2.cxx
               UserAnalysisHistos.cxx
               data/BeamDetMWPCDigi.cxx
               )

add_library(UserAnalysis SHARED ${SOURCE_LIB})

# ===============================================
# ExpertRoot
# BeamDet

# add_library(ERBeamDet SHARED IMPORTED)
# set_target_properties(ERBeamDet PROPERTIES IMPORTED_LOCATION $ENV{ERBUILD}/lib/libERBeamDet.so)

# target_link_libraries(UserAnalysis)
