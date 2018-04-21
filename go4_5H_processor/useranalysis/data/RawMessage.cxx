#include "RawMessage.h"

// STD
#include <iostream>
using std::cerr;
using std::endl;

// Project
#include "Support.h"

//TODO see comment below
RawMessage::RawMessage() :
	TObject(),
	mRawWord(0), // Yes zero here, because we want to clear the raw word with all zeros
	mEventType(-1),
	mEventSubtype(-1),
	mEventDummy(-1),
	mEventTrigger(-1),
	mEventCount(-1),
	mSubeventDlen(-1),
	mSubeventType(-1),
	mSubeventSubcrate(-1),
	mSubeventControl(-1),
	mSubeventFullID(-1),
	mSubeventProcID(-1),
	mSubsubeventVendor(-1),
	mSubsubeventGeo(-1),
	mSubsubeventModule(-1),
	mChannel(-1),
	mValueQA(-1),
	mValueT(-1),
	mSubsubeventFooterCounter(-1)
{
}

//TODO this copy constructor has to be tediously written correctly
// with all the data members copying. Cause if not - you will have empty
// data fields in the output objects even if you think that you have filled them
// during the unpacking stage.
// Moreover, you will have garbage if you do not zero all the members in the constructor.
RawMessage::RawMessage(const RawMessage &obj) :
	TObject(),
	mRawWord(obj.mRawWord),
	mEventType(obj.mEventType),
	mEventSubtype(obj.mEventSubtype),
	mEventDummy(obj.mEventDummy),
	mEventTrigger(obj.mEventTrigger),
	mEventCount(obj.mEventCount),
	mSubeventDlen(obj.mSubeventDlen),
	mSubeventType(obj.mSubeventType),
	mSubeventSubcrate(obj.mSubeventSubcrate),
	mSubeventControl(obj.mSubeventControl),
	mSubeventFullID(obj.mSubeventFullID),
	mSubeventProcID(obj.mSubeventProcID),
	mSubsubeventVendor(obj.mSubsubeventVendor),
	mSubsubeventGeo(obj.mSubsubeventGeo),
	mSubsubeventModule(obj.mSubsubeventModule),
	mChannel(obj.mChannel),
	mValueQA(obj.mValueQA),
	mValueT(obj.mValueT),
	mSubsubeventFooterCounter(obj.mSubsubeventFooterCounter)
{
}

RawMessage::~RawMessage()
{
}

void RawMessage::Dump(bool p_printEndl) const
{
	cerr << "Raw message:" << "\t"
	     << support::GetHexRepresentation(sizeof(Int_t), &mRawWord) << "\t"
	     << support::VendorAsString((support::enu_VENDOR)mSubsubeventVendor) << "\t" //TODO hack cast!!!
	     << "geo=" << mSubsubeventGeo << "\t"
	     << "module=" << mSubsubeventModule << "\t"
	     << "ch=" << mChannel << "\t"
	     << "valQA=" << mValueQA << "\t"
	     << "valT=" << mValueT;
	if (p_printEndl) cerr << endl;
}

void RawMessage::ExtDump(bool p_printEndl) const
{
	cerr << "Raw message:" << endl
	     << "\traw=" << support::GetHexRepresentation(sizeof(Int_t), &mRawWord) << endl
	     << "\tev_type=" << mEventType << endl
	     << "\tev_subtype=" << mEventSubtype << endl
	     << "\tev_dummy=" << mEventDummy << endl
	     << "\tev_trigger=" << mEventTrigger << endl
	     << "\tev_count=" << mEventCount << endl
	     << "\tsubev_dlen=" << mSubeventDlen << endl
	     << "\tsubev_type=" << mSubeventType << endl
	     << "\tsubev_subcrate=" << (Int_t)mSubeventSubcrate << endl
	     << "\tsubev_control=" << (Int_t)mSubeventControl << endl
	     << "\tsubev_fullID=" << mSubeventFullID << endl
	     << "\tsubev_procID=" << mSubeventProcID << endl
	     << "\tsubev_vendor=" << support::VendorAsString((support::enu_VENDOR)mSubsubeventVendor) << endl
	     << "\tsubev_geo=" << mSubsubeventGeo << endl
	     << "\tsubev_module=" << mSubsubeventModule << endl
	     << "\tchannel=" << mChannel << endl
	     << "\tvalueQA=" << mValueQA << endl
	     << "\tvalueT=" << mValueT << endl
	     << "\tfooter_counter=" << mSubsubeventFooterCounter << endl;
	if (p_printEndl) cerr << endl;
}

ClassImp(RawMessage)
