#ifndef USERPROC_H
#define USERPROC_H

#include <TGo4EventProcessor.h> // mother class

class TGo4EventElement;
class TGo4MbsEvent;
class TGo4MbsSubEvent;

class UserAnalysisHistos;
class UserEvent;

class UserProc : public TGo4EventProcessor
{
public:
	UserProc(const char* name = "UserProc");
	virtual ~UserProc();

	virtual Bool_t BuildEvent(TGo4EventElement* p_dest);

	void ProcessSubevent(TGo4MbsSubEvent* p_subevent);
	void ProcessSubeventRaw(Int_t p_size, Int_t* p_startAddress);

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

	ClassDef(UserProc, 1);
};

#endif // USERPROC_H
