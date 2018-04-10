#ifndef USERANALYSIS_H
#define USERANALYSIS_H

#include <TGo4Analysis.h> // mother class

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

	ClassDef(UserAnalysis, 1);
};

#endif // USERANALYSIS_H
