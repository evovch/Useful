/**

	@class RawMessage

*/

#ifndef RAWMESSAGE_H
#define RAWMESSAGE_H

#include <TObject.h> // mother class

class RawMessage : public TObject
{
public:
	RawMessage();
	RawMessage(const RawMessage &obj);
	virtual ~RawMessage();

	/**
	 * Short dump
	 */
	void Dump(bool p_printEndl = true) const;

	/**
	 * Extended dump
	 */
	void ExtDump(bool p_printEndl = true) const;

public:
	/**
	 * Raw 32-bit word as it comes.
	 * Note that headers are footers are saved and raw messages.
	 * Also note that a raw word itself does not represent full information
	 * because some values which are common for a group of raw messages
	 * should be taken from the [sub][sub]event headers
	 */
	Int_t mRawWord;

	/**
	 * From the event header
	 */
	Short_t mEventType;
	Short_t mEventSubtype;
	Short_t mEventDummy;
	Short_t mEventTrigger;
	Int_t   mEventCount;

	/**
	 * From the subevent header
	 */
	Int_t   mSubeventDlen;
	Short_t mSubeventType;
	Char_t  mSubeventSubcrate;
	Char_t  mSubeventControl;
	Int_t   mSubeventFullID;
	Short_t mSubeventProcID;

	/**
	 * From the subsubevent header
	 * See Support.h header
	 * {OTHER=0, MESYTEC=1, CAEN=2, AFFEAFFE=3}
	 */
	Char_t mSubsubeventVendor;

	/**
	 * From the subsubevent header
	 * CAEN only
	 * Geo is actually taken from each CAEN word, while module - only from the MESYTEC header
	 * Geo is written by the DAQ system into all types of words of the CAEN block
	 * (the header, data word, footer, etc.)
	 * Module is written only in the header of the MESYTEC block
	 */
	Int_t mSubsubeventGeo;

	/**
	 * From the subsubevent header
	 * MESYTEC only
	 * Geo is actually taken from each CAEN word, while module - only from the MESYTEC header
	 * Geo is written by the DAQ system into all types of words of the CAEN block
	 * (the header, data word, footer, etc.)
	 * Module is written only in the header of the MESYTEC block
	 */
	Int_t mSubsubeventModule;

	/**
	 * From the message
	 */
	Int_t mChannel;
	Int_t mValueQA;
	Int_t mValueT;

	/**
	 * From the subsubevent footer
	 * Both CAEN and MESYTEC write some counter in the footer
	 * though the formats are different
	 */
	Int_t mSubsubeventFooterCounter;

	ClassDef(RawMessage, 1);
};

#endif // RAWMESSAGE_H
