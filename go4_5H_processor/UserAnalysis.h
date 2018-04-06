#ifndef USERANALYSIS_H
#define USERANALYSIS_H

#include <TGo4Analysis.h> // mother class

class TGo4MbsEvent;

class UserAnalysis : public TGo4Analysis
{
public:
	UserAnalysis(const char* name = "UserAnalysis");
	UserAnalysis(int argc, char** argv);
	virtual ~UserAnalysis();

	void Construct(void);

	virtual Int_t UserPreLoop(void);
	virtual Int_t UserEventFunc(void);
	virtual Int_t UserPostLoop(void);

private:
	TGo4MbsEvent* fMbsEvent;
	Int_t fEvents;
	Int_t fLastEvent;

	ClassDef(UserAnalysis, 1);
};

#endif // USERANALYSIS_H
