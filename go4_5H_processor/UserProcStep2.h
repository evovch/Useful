#ifndef USERPROCSTEP2
#define USERPROCSTEP2

#include <TGo4EventProcessor.h> // mother class

class TGo4EventElement;

class UserProcStep2 : public TGo4EventProcessor
{
public:
	UserProcStep2(const char* name = "UserProcStep2");
	virtual ~UserProcStep2();

	virtual Bool_t BuildEvent(TGo4EventElement* p_dest);

private:
	unsigned long int mEventCounter;

	ClassDef(UserProcStep2, 1);
};

#endif // USERPROCSTEP2
