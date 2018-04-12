#include "RawMessage.h"

RawMessage::RawMessage() :
	TObject()
{
}

RawMessage::RawMessage(Int_t p_geo,
                       Int_t p_ch,
                       Int_t p_val) :
	mGeo(p_geo),
	mCh(p_ch),
	mVal(p_val)
{
}

RawMessage::~RawMessage()
{
}

ClassImp(RawMessage)
