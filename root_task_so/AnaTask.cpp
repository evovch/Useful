#include "AnaTask.h"

#include <iostream>
using std::cout;
using std::endl;

AnaTask::AnaTask() :
    TTask()
{
}

AnaTask::~AnaTask()
{
}

void AnaTask::Exec(Option_t* /*option*/)
{
	cout << "AnaTask::Exec()" << endl;
}

ClassImp(AnaTask)
