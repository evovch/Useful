#ifndef USERPROC_H
#define USERPROC_H

#include <TGo4EventProcessor.h> // mother class

class TGo4EventElement;
class TGo4MbsEvent;
class TGo4MbsSubEvent;

class UserAnalysisHistos;
class UserEvent;

enum enu_VENDOR {MESYTEC, CAEN, OTHER};

class UserProc : public TGo4EventProcessor
{
public:
	UserProc(const char* name = "UserProc");
	virtual ~UserProc();

	virtual Bool_t BuildEvent(TGo4EventElement* p_dest);

	void ProcessSubevent(TGo4MbsSubEvent* p_subevent);
	void ProcessSubeventRaw(Int_t p_size, Int_t* p_startAddress);

	void ProcessSubeventRawVME0(Int_t p_size, Int_t* p_startAddress);
	void ProcessSubeventRawVME1(Int_t p_size, Int_t* p_startAddress);
	void ProcessSubeventRawCAMAC(Int_t p_size, Int_t* p_startAddress);

	static enu_VENDOR CheckNextHeader(Int_t* p_startAddress);

	/**
		Return -1 if not found!
	*/
	static int FindCAENfooter(Int_t p_maxSize, Int_t* p_startAddress);

	/**
		Return -1 if not found!
	*/
	static int FindMESYTECfooter(Int_t p_maxSize, Int_t* p_startAddress);

	void ProcessSubsubevent_MESYTEC(Int_t p_size, Int_t* p_startAddress);

	void ProcessSubsubevent_CAEN(Int_t p_size, Int_t* p_startAddress);

	//void ProcessSubeventRaw_CAEN(Int_t* p_startAddress, Int_t p_cursor);
	//void ProcessSubeventRaw_MESYTEC(Int_t* p_startAddress, Int_t p_cursor);

	void DumpEventHeader(TGo4MbsEvent* p_inp_evt) const;
	void DumpSubeventData(Int_t p_size, Int_t* p_startAddress) const;

private:
	unsigned long int mEventCounter;

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

	ClassDef(UserProc, 1);
};

#endif // USERPROC_H
