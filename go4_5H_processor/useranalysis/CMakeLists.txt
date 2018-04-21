# =============================================================================
# ROOT dictionary

# Normally this is not needed, but rootcling cannot see these files
# if the current source directory (which is 'PROJECTBASEDIR/useranalysis' in our case)
# is not added to the include_directories list.
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR})

set(ROOTCLING_HEADERS UserAnalysis.h
                      UserParameter.h
                      unpacking/UserEventUnpacking.h
                      unpacking/UserProcUnpacking.h
                      unpacking/UserHistosUnpacking.h
                      monitoring/UserEventMonitoring.h
                      monitoring/UserEventNew.h
                      monitoring/UserEventLeftTele.h
                      monitoring/UserEventRightTele.h
                      monitoring/UserProcMonitoring.h
                      monitoring/UserHistosMonitoring.h
                      step2/UserEventStep2.h
                      step2/UserProcStep2.h
                      data/BeamDetMWPCDigi.h
                      data/RawMessage.h
                      )

ROOT_GENERATE_DICTIONARY(G__UserAnalysis ${ROOTCLING_HEADERS} LINKDEF UserAnalysisLinkDef.h)

# =============================================================================

include_directories(${CMAKE_SOURCE_DIR})

set(SOURCE_LIB G__UserAnalysis.cxx
               UserAnalysis.cxx
               UserParameter.cxx
               unpacking/UserEventUnpacking.cxx
               unpacking/UserProcUnpacking.cxx
               unpacking/UserHistosUnpacking.cxx
               monitoring/UserEventMonitoring.cxx
               monitoring/UserEventNew.cxx
               monitoring/UserEventLeftTele.cxx
               monitoring/UserEventRightTele.cxx
               monitoring/UserProcMonitoring.cxx
               monitoring/UserHistosMonitoring.cxx
               step2/UserEventStep2.cxx
               step2/UserProcStep2.cxx
               data/BeamDetMWPCDigi.cxx
               data/RawMessage.cxx
               Support.cxx
               )

add_library(UserAnalysis SHARED ${SOURCE_LIB})

target_link_libraries(UserAnalysis Go4Analysis)

target_link_libraries(UserAnalysis SetupConfigCppWrapper)
