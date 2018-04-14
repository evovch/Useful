/**

	@class UserProcMonitoring

*/

#ifndef USERPROCMONITORING_H
#define USERPROCMONITORING_H

#include <TGo4EventProcessor.h> // mother class

class TGo4EventElement;

class UserEventMonitoring;
class UserHistosMonitoring;
class RawMessage;

class UserProcMonitoring : public TGo4EventProcessor
{
public:
	UserProcMonitoring(const char* name = "UserProcMonitoring");
	virtual ~UserProcMonitoring();

	virtual Bool_t BuildEvent(TGo4EventElement* p_dest);

private:
	/**
	 *
	 */
	void ProcessMESYTECmessage(const RawMessage* p_message);

	/**
	 *
	 */
	void ProcessMESYTECmessageVME0(const RawMessage* p_message);
	/**
	 *
	 */
	void ProcessMESYTECmessageVME1(const RawMessage* p_message);

	/**
	 *
	 */
	void ProcessCAENmessage(const RawMessage* p_message);

	/**
	 *
	 */
	void ProcessCAENmessageVME0(const RawMessage* p_message);

	/**
	 *
	 */
	void ProcessCAENmessageVME1(const RawMessage* p_message);

private:
	/**
	 *
	 */
	unsigned long int mEventCounter;

	/**
	 *
	 */
	UserEventMonitoring* mCurrentOutputEvent;

	/**
	 * Put all your output histograms inside this object.
	 * See UserHistosMonitoring class.
	 */
	UserHistosMonitoring* mHistoMan;

	ClassDef(UserProcMonitoring, 1);
};

#endif // USERPROCMONITORING_H
