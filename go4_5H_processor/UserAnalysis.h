/**

	@class UserAnalysis

*/

#ifndef USERANALYSIS_H
#define USERANALYSIS_H

#include <TGo4Analysis.h> // mother class

//TODO not used by now
class TGo4MbsEvent;
class UserEventUnpack;
class UserEventMonitoring;
class UserEventStep2;

class UserAnalysis : public TGo4Analysis
{
public:
	UserAnalysis(const char* name = "UserAnalysis");
	UserAnalysis(int argc, char** argv);
	virtual ~UserAnalysis();

	void Construct(void);

	virtual Int_t UserPreLoop();
	virtual Int_t UserEventFunc();
	virtual Int_t UserPostLoop();

private:
	unsigned long int mEventCounter;

	//TODO not used by now
	TGo4MbsEvent* mMbsEvent;
	UserEventUnpack* mUserEventUnpack;
	UserEventMonitoring* mUserEventMonitoring;
	UserEventStep2* mUserEventStep2;

	ClassDef(UserAnalysis, 1);
};

#endif // USERANALYSIS_H
