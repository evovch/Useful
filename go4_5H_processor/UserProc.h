/**

	@class UserProc

*/

#ifndef USERPROC_H
#define USERPROC_H

#include <TGo4EventProcessor.h> // mother class

class TGo4EventElement;
class TGo4MbsEvent;
class TGo4MbsSubEvent;

class UserAnalysisHistos;
class UserEvent;

enum enu_VENDOR {MESYTEC, CAEN, OTHER, AFFEAFFE};

class UserProc : public TGo4EventProcessor
{
public:
	UserProc(const char* name = "UserProc");
	virtual ~UserProc();

	virtual Bool_t BuildEvent(TGo4EventElement* p_dest);

	void ProcessSubevent(TGo4MbsSubEvent* p_subevent);

	void ProcessSubeventRaw(Int_t p_size, const Int_t* p_startAddress);
	void ProcessSubeventRawVME0(Int_t p_size, const Int_t* p_startAddress);
	void ProcessSubeventRawVME1(Int_t p_size, const Int_t* p_startAddress);
	void ProcessSubeventRawCAMAC(Int_t p_size, const Int_t* p_startAddress);
	void ProcessSubeventRawCAMACmwpc(Int_t p_size, const Int_t* p_startAddress);

	void ProcessSubsubevent_MESYTEC(Int_t p_size, const Int_t* p_startAddress);
	void ProcessSubsubevent_CAEN(Int_t p_size, const Int_t* p_startAddress);

private:
	//NOTE: static methods do not change any class data members.
	// Here they basically look into the data and do something
	// nice and clean - find something and  return found word position,
	// check the type of the word,
	// print something on the screen, etc.
	// no counters are (at least should be) changed

	/**
	 *
	 */
	static enu_VENDOR CheckNextHeader(const Int_t* p_startAddress);

	/**
	 * Return -1 if not found!
	 */
	static Int_t FindCAENfooter(Int_t p_maxSize, const Int_t* p_startAddress);

	/**
	 * Return -1 if not found!
	 */
	static Int_t FindMESYTECfooter(Int_t p_maxSize, const Int_t* p_startAddress);

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

private:
	unsigned long int mEventCounter;

	unsigned long int mHeadersWords;
	unsigned long int mNknownWords;
	unsigned long int mNunknownWords;

	UserEvent* mCurrentOutputEvent;

	/**
	 * Put all your output histograms and graphs inside this object.
	 * See UserAnalysisHistos class
	 */
	UserAnalysisHistos* mHistoMan;

	/**
		This flag is set true when a header is found and
		false when a footer is found
	*/
	static bool mInsidePackage;

	/**
	 * Summary stream
	 */
	FILE* mFileSummary;

	ClassDef(UserProc, 1);
};

#endif // USERPROC_H
