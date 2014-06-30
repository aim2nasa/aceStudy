#ifndef __CTASK_H__
#define __CTASK_H__

#include <ace/Task.h>
#include <ace/Synch.h>

typedef ACE_Task<ACE_MT_SYNCH> Task_Base;

class CTask : public Task_Base
{
public:
	typedef Task_Base inherited;

	CTask(const char *name,int numberOfThreads);

	virtual ~CTask (void);

	virtual int open (void *arg);
	virtual int close (u_long flags);
	virtual int svc (void);
	virtual int put (ACE_Message_Block *message,ACE_Time_Value *timeout);

	const char *name(void) const;

protected:
	int			m_nNumberOfThreads;
	char		m_name[128];
	ACE_Barrier	m_barrier;
};

#endif