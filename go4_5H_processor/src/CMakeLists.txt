# =============================================================================
# ROOT dictionary

# Normally this is not needed, but rootcling cannot see these files
# if the current source directory (which is 'PROJECTBASEDIR/src' in our case)
# is not added to the include_directories list.
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

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

# =============================================================================

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
