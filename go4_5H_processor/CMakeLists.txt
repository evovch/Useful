# ===============================================

# https://root.cern.ch/how/integrate-root-my-project-cmake

cmake_minimum_required(VERSION 3.5)

project(go4_5H_processor)

# ===============================================
# External packages

# ROOT
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
find_package(ROOT REQUIRED)

#---Define useful ROOT functions and macros (e.g. ROOT_GENERATE_DICTIONARY)
include(${ROOT_USE_FILE})

# Go4
include_directories($ENV{GO4SYS}/include)

# ===============================================

# if (CMAKE_COMPILER_IS_GNUCXX)
#     set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wextra -Wpedantic -std=c++11")
# endif()

# ===============================================

set(ROOTCLING_HEADERS UserAnalysis.h
                      UserEvent.h
                      UserProc.h
                      UserEventStep2.h
                      UserProcStep2.h
                      )

ROOT_GENERATE_DICTIONARY(G__UserAnalysis ${ROOTCLING_HEADERS} LINKDEF UserAnalysisLinkDef.h)

set(SOURCE_LIB G__UserAnalysis.cxx
               UserAnalysis.cxx
               UserEvent.cxx
               UserProc.cxx
               UserEventStep2.cxx
               UserProcStep2.cxx
               )

add_library(UserAnalysis SHARED ${SOURCE_LIB})
