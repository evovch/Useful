/**

	@class UserEventMonitoring

*/

#ifndef USEREVENTMONITORING_H
#define USEREVENTMONITORING_H

#include <TGo4EventElement.h> // mother class

class TClonesArray;

class UserEventMonitoring : public TGo4EventElement
{
	friend class UserProcMonitoring; // provider

public:
	UserEventMonitoring(const char* name = "UserEventMonitoring");
	virtual ~UserEventMonitoring();

	void Clear(Option_t* t = "");

	void Dump(void) const;

public: //TODO should be private. Made public for ER

	UShort_t  CsI_L[16];
	UShort_t tCsI_L[16];
	UShort_t  CsI_R[16];
	UShort_t tCsI_R[16];
	UShort_t  SQX_L[32];
	UShort_t tSQX_L[32];
	UShort_t  SQY_L[16];
	UShort_t tSQY_L[16];
	UShort_t  SQX_R[32];
	UShort_t tSQX_R[32];
	UShort_t  SQY_R[16];
	UShort_t tSQY_R[16];
	UShort_t  SQ20[16];
	UShort_t tSQ20[16];

	UShort_t neutAmp[32];
	UShort_t neutTAC[32];
	UShort_t neutTDC[32];

	UShort_t  F3[4];
	UShort_t tF3[4];
	UShort_t  F5[4];
	UShort_t tF5[4];
	UShort_t  F6[4];
	UShort_t tF6[4];
	UShort_t tMWPC[4];

	ClassDef(UserEventMonitoring, 1);
};

#endif // USEREVENTMONITORING_H
