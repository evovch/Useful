/**

	@class UserAnalysisHistos

*/

#ifndef USERANALYSISHISTOS_H
#define USERANALYSISHISTOS_H

#include <TObject.h> // mother class

class TH1;
class TH2;
//class TGraph; // Does not work by now

class UserAnalysisHistos : public TObject
{
public:
	UserAnalysisHistos();
	~UserAnalysisHistos();

public:
	TH1* mHisto1;
	TH2* mHisto2;
	//TGraph* mGraph; // Does not work by now

	ClassDef(TObject, 1);
};

#endif // USERANALYSISHISTOS_H
