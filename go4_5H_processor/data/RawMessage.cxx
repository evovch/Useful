#include "RawMessage.h"

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

ClassImp(RawMessage)
