#ifndef ANATASK_H
#define ANATASK_H

#include "TTask.h" // mother class

class AnaTask : public TTask
{
public:
    AnaTask();
    virtual ~AnaTask();

    virtual void Exec(Option_t* option);

    ClassDef(AnaTask, 1);
};

#endif // ANATASK_H
