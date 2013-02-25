#ifndef __CTASK_H__
#define __CTASK_H__

#include "ace/Task.h"
#include "ace/Barrier.h"

class Task : public ACE_Task < ACE_MT_SYNCH >
{
public:
    typedef ACE_Task < ACE_MT_SYNCH > inherited;

    Task (int sub_tasks = 0);
    ~Task (void);

    int start (int threads = 1);
    int svc (void);
    int close (u_long flags = 0);

protected:
    ACE_Barrier * barrier_;
};

#endif