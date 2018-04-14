# ===============================================

cmake_minimum_required(VERSION 3.5)

# Give your project a name
project(go4_5H_processor)

# Switch in on for more verbose make output
# set(CMAKE_VERBOSE_MAKEFILE ON)

# ===============================================
# External packages

# See: https://root.cern.ch/how/integrate-root-my-project-cmake

# ROOT
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
find_package(ROOT REQUIRED)

# Define useful ROOT functions and macros (e.g. ROOT_GENERATE_DICTIONARY)
include(${ROOT_USE_FILE})

# Go4
if (DEFINED ENV{GO4SYS})
    message("Go4 found at $ENV{GO4SYS}")
    include_directories($ENV{GO4SYS}/include)
    link_directories($ENV{GO4SYS}/lib)
else()
    message(FATAL_ERROR "Go4 not found! Please, run go4login or define the GO4SYS env. variable.")
endif()

# ===============================================

#if (CMAKE_COMPILER_IS_GNUCXX)
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wextra -Wpedantic")
#    # -std=c++11
#endif()

# ===============================================
# ROOT dictionary

include_directories(${CMAKE_SOURCE_DIR})

set(ROOTCLING_HEADERS UserAnalysis.h
                      UserEventUnpacking.h
                      UserProcUnpacking.h
                      UserHistosUnpacking.h
                      UserEventMonitoring.h
                      UserProcMonitoring.h
                      UserHistosMonitoring.h
                      UserEventStep2.h
                      UserProcStep2.h
                      data/BeamDetMWPCDigi.h
                      data/RawMessage.h
                      )

ROOT_GENERATE_DICTIONARY(G__UserAnalysis ${ROOTCLING_HEADERS} LINKDEF UserAnalysisLinkDef.h)

# ===============================================

set(SOURCE_LIB G__UserAnalysis.cxx
               UserAnalysis.cxx
               UserEventUnpacking.cxx
               UserProcUnpacking.cxx
               UserHistosUnpacking.cxx
               UserEventMonitoring.cxx
               UserProcMonitoring.cxx
               UserHistosMonitoring.cxx
               UserEventStep2.cxx
               UserProcStep2.cxx
               data/BeamDetMWPCDigi.cxx
               data/RawMessage.cxx
               Support.cxx
               )

add_library(UserAnalysis SHARED ${SOURCE_LIB})

target_link_libraries(UserAnalysis Go4Analysis)
