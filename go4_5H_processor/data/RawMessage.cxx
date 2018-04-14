#include "RawMessage.h"

// STD
#include <iostream>
using std::cerr;
using std::endl;

// Project
#include "Support.h"

RawMessage::RawMessage() :
	TObject(),
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

RawMessage::RawMessage(const RawMessage &obj) :
	TObject(),
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
