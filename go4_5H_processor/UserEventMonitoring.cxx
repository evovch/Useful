#include "UserEventMonitoring.h"

UserEventMonitoring::UserEventMonitoring(const char* name) :
	TGo4EventElement(name)
{
}

UserEventMonitoring::~UserEventMonitoring()
{
}

void UserEventMonitoring::Clear(Option_t* t)
{
	//TODO zero all data members!

	for (UInt_t i=0; i<16; i++) {
		 CsI_L[i] = 0;
		tCsI_L[i] = 0;
		 CsI_R[i] = 0;
		tCsI_R[i] = 0;
		 SQY_L[i] = 0;
		tSQY_L[i] = 0;
		 SQY_R[i] = 0;
		tSQY_R[i] = 0;
		  SQ20[i] = 0;
		 tSQ20[i] = 0;
	}
	for (UInt_t i=0; i<32; i++) {
		 SQX_L[i] = 0;
		tSQX_L[i] = 0;
		 SQX_R[i] = 0;
		tSQX_R[i] = 0;
		neutAmp[i] = 0;
		neutTAC[i] = 0;
		neutTDC[i] = 0;
	}


	for (UInt_t i=0; i<4; i++) {
		 F3[i] = 0;
		tF3[i] = 0;
		 F5[i] = 0;
		tF5[i] = 0;
		 F6[i] = 0;
		tF6[i] = 0;
		tMWPC[i] = 0;
	}
}

ClassImp(UserEventMonitoring)
