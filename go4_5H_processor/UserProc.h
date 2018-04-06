#ifndef USERPROC_H
#define USERPROC_H

#include <TGo4EventProcessor.h> // mother class

class TGo4EventElement;

class UserProc : public TGo4EventProcessor
{
public:
	UserProc(const char* name = "UserProc");
	virtual ~UserProc();

	Bool_t BuildEvent(TGo4EventElement* p_dest);

	ClassDef(UserProc, 1);
};

#endif // USERPROC_H
