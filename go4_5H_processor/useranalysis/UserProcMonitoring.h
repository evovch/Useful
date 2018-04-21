/**

	@class UserProcMonitoring

*/

#ifndef USERPROCMONITORING_H
#define USERPROCMONITORING_H

#include <TGo4EventProcessor.h> // mother class

#include "setupconfigcppwrapper/SetupConfiguration.h"

class TGo4EventElement;

class UserEventMonitoring;
class UserHistosMonitoring;
class UserEventUnpacking;
class RawMessage;

class UserProcMonitoring : public TGo4EventProcessor
{
public: // methods

	UserProcMonitoring(const char* name = "UserProcMonitoring");
	virtual ~UserProcMonitoring();

	virtual Bool_t BuildEvent(TGo4EventElement* p_dest);

private: // methods
	/**
	 * Process raw messages from MESYTEC and CAEN electronics blocks
	 */
	void ProcessMessageUniversal(const RawMessage* p_message);

	/**
	 * Process raw CAMAC words assuming they have been produced by MWPCs
	 */
	void ProcessCAMACmwpcWords(const UserEventUnpacking* p_inputEvent);

private: // data members
	/**
	 * Counter or processed events
	 */
	unsigned long int mEventCounter;

	/**
	 * Current output event object. This object is filled during BuildEvent
	 * and then pushed into the output tree by the framework.
	 */
	UserEventMonitoring* mCurrentOutputEvent;

	/**
	 * Put all your output histograms inside this object.
	 * See UserHistosMonitoring class.
	 */
	UserHistosMonitoring* mHistoMan;

	/**
	 * Setup configuration object.
	 */
	SetupConfiguration* mSetupConfiguration;

	ClassDef(UserProcMonitoring, 1);
};

#endif // USERPROCMONITORING_H
