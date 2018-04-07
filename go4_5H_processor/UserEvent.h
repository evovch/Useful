#ifndef USEREVENT_H
#define USEREVENT_H

#include <TGo4EventElement.h> // mother class

class UserEvent : public TGo4EventElement
{
public:
	UserEvent(const char* name = "UserEvent");
	virtual ~UserEvent();

	void Clear(Option_t* t = "");

public:
	Int_t mSubcrate;
	Int_t mControl;
	Short_t mProcID;

	ClassDef(UserEvent, 1);
};

#endif // USEREVENT_H
