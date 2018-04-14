/**

	@class UserProcUnpacking

*/

#ifndef USERPROCUNPACKING_H
#define USERPROCUNPACKING_H

#include <TGo4EventProcessor.h> // mother class

// Project
#include "Support.h"
#include "data/RawMessage.h"

class TGo4EventElement;
class TGo4MbsEvent;
class TGo4MbsSubEvent;

class UserHistosUnpacking;
class UserEventUnpacking;

class UserProcUnpacking : public TGo4EventProcessor
{
public: // methods

	UserProcUnpacking(const char* name = "UserProcUnpacking");
	virtual ~UserProcUnpacking();

	virtual Bool_t BuildEvent(TGo4EventElement* p_dest);

	void ProcessSubevent(TGo4MbsSubEvent* p_subevent);

	void ProcessSubeventRaw(Int_t p_size, const Int_t* p_startAddress);
	void ProcessSubeventRawVME0(Int_t p_size, const Int_t* p_startAddress);
	void ProcessSubeventRawVME1(Int_t p_size, const Int_t* p_startAddress);
	/*void ProcessSubeventRawCAMAC(Int_t p_size, const Int_t* p_startAddress);*/
	void ProcessSubeventRawCAMACmwpc(Int_t p_size, const Int_t* p_startAddress);

	void ProcessSubsubevent_MESYTEC(Int_t p_size, const Int_t* p_startAddress);
	void ProcessSubsubevent_CAEN(Int_t p_size, const Int_t* p_startAddress);

private:

	/**
	 *
	 */
	static RawMessage mCurMessage;

	/**
	 *
	 */
	void PushOutputRawMessage(void);

	/**
	 * Output event object.
	 * This object is cleared at the end of the previous event.
	 * Then it is filled during BuildEvent() and all the method
	 * which are called by BuildEvent() and then this object
	 * is written out by the framework after BuildEvent()
	 */
	UserEventUnpacking* mCurrentOutputEvent;

	/**
	 * Basically - start event
	 * Set event data of the current output message
	 * according to the unpacked values
	 */
	void ProcessEventHeader(TGo4MbsEvent* p_event);

	/**
	 * Obviously - finish event
	 * Set event data of the current output message to -1
	 */
	void FinishEvent();

	/**
	 * Basically - start subevent
	 * Set subevent data of the current output message
	 * according to the unpacked values
	 */
	void ProcessSubeventHeader(TGo4MbsSubEvent* p_subevent);

	/**
	 * Obviously - finish subevent
	 * Set subevent data of the current output message to -1
	 */
	void FinishSubevent();

private: // static methods

	//NOTE: static methods do not change any class data members.
	// Here they basically look into the data and do something
	// nice and clean - find something and return found word position,
	// check the type of the word,
	// print something on the screen, etc.
	// no counters are (at least should be) changed

	/**
	 *
	 */
	static support::enu_VENDOR CheckNextHeader(const Int_t* p_startAddress);

	/**
	 * Return the position of the footer.
	 * Return -1 if not found!
	 * The counter from the footer is written into o_counter.
	 */
	static Int_t FindMESYTECfooter(Int_t p_maxSize, const Int_t* p_startAddress, Int_t* o_counter);

	/**
	 * Return the position of the footer.
	 * Return -1 if not found!
	 * The counter from the footer is written into o_counter.
	 */
	static Int_t FindCAENfooter(Int_t p_maxSize, const Int_t* p_startAddress, Int_t* o_counter);

	/**
	 *
	 */
	static Int_t ExtractCounterFromMESYTECfooter(Int_t p_word) { return (p_word & 0x3fffffff); } // 30 bits

	/**
	 *
	 */
	static Int_t ExtractCounterFromCAENfooter(Int_t p_word) { return (p_word & 0xffffff); } // 24 bits

	/**
	 *
	 */
	static void DumpEventHeader(/*const*/ TGo4MbsEvent* p_event);

	/**
	 *
	 */
	static void DumpSubeventHeader(/*const*/ TGo4MbsSubEvent* p_subevent);

	/**
	 *
	 */
	static void DumpSubeventData(Int_t p_size, const Int_t* p_startAddress);

	/**
	 * The same but just in different format
	 */
	static void DumpSubeventData2(Int_t p_size, const Int_t* p_startAddress);

private: // data members

	/**
	 * Unpacking statistics
	 */
	unsigned long int mEventCounter;
	unsigned long int mSubEventCounter;
	unsigned long int mHeadersWords;
	unsigned long int mNknownWords;
	unsigned long int mNunknownWords;

	/**
	 * Put all your output histograms inside this object.
	 * See UserHistosUnpacking class.
	 */
	UserHistosUnpacking* mHistoMan;

	/**
	 * This flag is set true when a header is found and false when a footer is found.
	 * It is used to track the messages which come outside of the subsubevent block
	 * (not between the header and the footer).
	 */
	static bool mInsidePackage;

	/**
	 * Summary stream
	 */
	FILE* mFileSummary;

	ClassDef(UserProcUnpacking, 1);
};

#endif // USERPROCUNPACKING_H
