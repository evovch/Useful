#include "classes.h"

// -----------------------------------------------------------------------------------------------

ClassImp(cls_hit)

cls_hit::cls_hit(void) :
	ts(0), ch(0), adc(0)
{
}

cls_hit::cls_hit(UInt_t p_ts, UInt_t p_ch, UInt_t p_adc) :
	ts(p_ts), ch(p_ch), adc(p_adc)
{
}

cls_hit::~cls_hit(void)
{
	Clear();
}

void cls_hit::Clear(Option_t* /*option*/)
{
	TObject::Clear();
}

void cls_hit::Set(UInt_t p_ts, UInt_t p_ch, UInt_t p_adc)
{
	ts = p_ts;
	ch = p_ch;
	adc = p_adc;
}

// -----------------------------------------------------------------------------------------------

ClassImp(cls_event)

ULong_t cls_event::fEventsCounter = 0;

cls_event::cls_event(void)
{
	fNumHits = 0;
	fHits = new TClonesArray("cls_hit", 10);
}

cls_event::~cls_event(void)
{
	Clear();
}

void cls_event::Clear(Option_t* /*option*/)
{
	fHits->Clear("C");
	fNumHits = 0;
}

cls_hit* cls_event::AddHit(UInt_t p_ts, UInt_t p_ch, UInt_t p_adc)
{
#if ROOT_VERSION_CODE >= ROOT_VERSION(5,32,0)
 	cls_hit *hit = (cls_hit*)fHits->ConstructedAt(fNumHits++);
 	hit->Set(p_ts, p_ch, p_adc);
#else
 	TClonesArray &hits = *fHits;
 	cls_hit *hit = new(hits[fNumHits++]) cls_hit(p_ts, p_ch, p_adc);
#endif
    return hit;
}

void cls_event::NextEvent(void)
{
	fEventId = fEventsCounter;
	fEventsCounter++;
}

// -----------------------------------------------------------------------------------------------
