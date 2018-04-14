/**

	@class UserHistosUnpacking

*/

#ifndef USERHISTOSUNPACKING_H
#define USERHISTOSUNPACKING_H

#include <TObject.h> // mother class

class TH1;
class TH2;
//class TGraph; // Does not work by now

class UserHistosUnpacking : public TObject
{
public:
	UserHistosUnpacking();
	~UserHistosUnpacking();

public:
	TH1* mHisto1;
	TH2* mHisto2;
	//TGraph* mGraph; // Does not work by now

	ClassDef(TObject, 1);
};

#endif // USERHISTOSUNPACKING_H
