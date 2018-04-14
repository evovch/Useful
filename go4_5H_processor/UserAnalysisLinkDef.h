#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

// Manager
#pragma link C++ class UserAnalysis+;

// Data
#pragma link C++ class BeamDetMWPCDigi+;
#pragma link C++ class RawMessage+;

// Unpacking step
#pragma link C++ class UserEventUnpacking+;
#pragma link C++ class UserProcUnpacking+;
#pragma link C++ class UserHistosUnpacking+;

// Monitoring step
#pragma link C++ class UserEventMonitoring+;
#pragma link C++ class UserProcMonitoring+;
#pragma link C++ class UserHistosMonitoring+;

// Analysis step
#pragma link C++ class UserEventStep2+;
#pragma link C++ class UserProcStep2+;

#endif // __CINT__
