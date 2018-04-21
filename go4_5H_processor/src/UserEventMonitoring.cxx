#include "UserEventMonitoring.h"

// STD
#include <iostream>
using std::cerr;
using std::endl;

UserEventMonitoring::UserEventMonitoring(const char* name) :
	TGo4EventElement(name)
{
	//cerr << "UserEventMonitoring::UserEventMonitoring() -> ";
	this->Clear();
}

UserEventMonitoring::~UserEventMonitoring()
{
}

void UserEventMonitoring::Clear(Option_t* t)
{
	//TODO zero all data members!
	//cerr << "UserEventMonitoring::Clear()" << endl;

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

void UserEventMonitoring::Dump(void) const
{
	//TODO dump all data members!

	cerr << " CsI_L\t"; for (UInt_t i=0; i<16; i++) { cerr <<  CsI_L[i] << "\t"; } cerr << endl;
	cerr << "tCsI_L\t"; for (UInt_t i=0; i<16; i++) { cerr << tCsI_L[i] << "\t"; } cerr << endl;
	cerr << " CsI_R\t"; for (UInt_t i=0; i<16; i++) { cerr <<  CsI_R[i] << "\t"; } cerr << endl;
	cerr << "tCsI_R\t"; for (UInt_t i=0; i<16; i++) { cerr << tCsI_R[i] << "\t"; } cerr << endl;

	cerr << " SQX_L\t"; for (UInt_t i=0; i<16; i++) { cerr <<  SQX_L[i] << "\t"; } cerr << endl;
	cerr << "      \t"; for (UInt_t i=0; i<16; i++) { cerr <<  SQX_L[i+16] << "\t"; } cerr << endl;
	cerr << "tSQX_L\t"; for (UInt_t i=0; i<16; i++) { cerr << tSQX_L[i] << "\t"; } cerr << endl;
	cerr << "      \t"; for (UInt_t i=0; i<16; i++) { cerr << tSQX_L[i+16] << "\t"; } cerr << endl;
	cerr << " SQY_L\t"; for (UInt_t i=0; i<16; i++) { cerr <<  SQY_L[i] << "\t"; } cerr << endl;
	cerr << "tSQY_L\t"; for (UInt_t i=0; i<16; i++) { cerr << tSQY_L[i] << "\t"; } cerr << endl;

	cerr << " SQX_R\t"; for (UInt_t i=0; i<16; i++) { cerr <<  SQX_R[i] << "\t"; } cerr << endl;
	cerr << "      \t"; for (UInt_t i=0; i<16; i++) { cerr <<  SQX_R[i+16] << "\t"; } cerr << endl;
	cerr << "tSQX_R\t"; for (UInt_t i=0; i<16; i++) { cerr << tSQX_R[i] << "\t"; } cerr << endl;
	cerr << "      \t"; for (UInt_t i=0; i<16; i++) { cerr << tSQX_R[i+16] << "\t"; } cerr << endl;
	cerr << " SQY_R\t"; for (UInt_t i=0; i<16; i++) { cerr <<  SQY_R[i] << "\t"; } cerr << endl;
	cerr << "tSQY_R\t"; for (UInt_t i=0; i<16; i++) { cerr << tSQY_R[i] << "\t"; } cerr << endl;

	cerr << "  SQ20\t"; for (UInt_t i=0; i<16; i++) { cerr <<   SQ20[i] << "\t"; } cerr << endl;
	cerr << " tSQ20\t"; for (UInt_t i=0; i<16; i++) { cerr <<  tSQ20[i] << "\t"; } cerr << endl;
}

UShort_t* UserEventMonitoring::GetFieldByName(TString p_name)
{
	if      (p_name ==  "CsI_L")    { return  CsI_L; }
	else if (p_name == "tCsI_L")    { return tCsI_L; }
	else if (p_name ==  "CsI_R")    { return  CsI_R; }
	else if (p_name == "tCsI_R")    { return tCsI_R; }
	else if (p_name ==  "SQX_L")    { return  SQX_L; }
	else if (p_name == "tSQX_L")    { return tSQX_L; }
	else if (p_name ==  "SQY_L")    { return  SQY_L; }
	else if (p_name == "tSQY_L")    { return tSQY_L; }
	else if (p_name ==  "SQX_R")    { return  SQX_R; }
	else if (p_name == "tSQX_R")    { return tSQX_R; }
	else if (p_name ==  "SQY_R")    { return  SQY_R; }
	else if (p_name == "tSQY_R")    { return tSQY_R; }
	else if (p_name ==  "SQ20")     { return  SQ20; }
	else if (p_name == "tSQ20")     { return tSQ20; }
	else if (p_name ==  "neutAmp")  { return  neutAmp; }
	else if (p_name ==  "neutTAC")  { return  neutTAC; }
	else if (p_name ==  "neutTDC")  { return  neutTDC; }
	else if (p_name ==  "F3")       { return  F3; }
	else if (p_name == "tF3")       { return tF3; }
	else if (p_name ==  "F5")       { return  F5; }
	else if (p_name == "tF5")       { return tF5; }
	else if (p_name ==  "F6")       { return  F6; }
	else if (p_name == "tF6")       { return tF6; }
	else if (p_name == "tMWPC")     { return tMWPC; }
	/*else if (p_name == "Unknown") {
		cerr << "[WARN  ] Acquiring en event data field '" << p_name << "' which does not exist." << endl;
		return NULL; }*/
	else {
		cerr << "[ERROR ] Acquiring en event data field '" << p_name << "' which does not exist." << endl;
		return NULL;
	}
}

ClassImp(UserEventMonitoring)
