/**

	@class UserHistosMonitoring

*/

#ifndef USERHISTOSMONITORING_H
#define USERHISTOSMONITORING_H

#include <TObject.h> // mother class

class TH1;
class TH2;

class UserHistosMonitoring : public TObject
{
public:
	UserHistosMonitoring();
	~UserHistosMonitoring();

public:

	TH1* d1F3[4];
	TH1* d1tF3[4];
	TH1* d1F5[4];
	TH1* d1tF5[4];
	TH1* d1F6[4];
	TH1* d1tF6[4];
	TH1* d1tMWPC[4];

	ClassDef(UserHistosMonitoring, 1);
};

#endif // USERHISTOSMONITORING_H
