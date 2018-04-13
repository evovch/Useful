/**

	@class UserProcMonitoring

*/

#ifndef USERPROCMONITORING_H
#define USERPROCMONITORING_H

#include <TGo4EventProcessor.h> // mother class

class TGo4EventElement;

class UserProcMonitoring : public TGo4EventProcessor
{
public:
	UserProcMonitoring(const char* name = "UserProcMonitoring");
	virtual ~UserProcMonitoring();

	virtual Bool_t BuildEvent(TGo4EventElement* p_dest);

private:
	unsigned long int mEventCounter;

	ClassDef(UserProcMonitoring, 1);
};

#endif // USERPROCMONITORING_H
