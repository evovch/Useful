#ifndef _CLASSES_H_
#define _CLASSES_H_

#include <TObject.h>
#include <TClonesArray.h>

class cls_hit : public TObject
{
public:
	cls_hit(void);
	cls_hit(UInt_t p_ts, UInt_t p_ch, UInt_t p_adc);
	virtual ~cls_hit(void);

	void Clear(Option_t *option="");

	void Set(UInt_t p_ts, UInt_t p_ch, UInt_t p_adc);

private:
	UInt_t ts;
	UInt_t ch;
	UInt_t adc;

public:
	ClassDef(cls_hit, 1);
};

class cls_event : public TObject
{
public:
	cls_event(void);
	virtual ~cls_event(void);

	void Clear(Option_t *option="");

	cls_hit* AddHit(UInt_t p_ts, UInt_t p_ch, UInt_t p_adc);
	void SetTimestamp(Double_t p_ts) { fTimestamp = p_ts; }
	void NextEvent(void);

private:
	ULong_t fEventId;
	Double_t fTimestamp;

	TClonesArray* fHits;	//->
	UInt_t fNumHits;

	static ULong_t fEventsCounter;

public:
	ClassDef(cls_event, 1);
};

#endif // _CLASSES_H_