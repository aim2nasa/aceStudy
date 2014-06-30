#ifndef __CENDTASK_H__
#define __CENDTASK_H__

#include "CTask.h"

class CEndTask : public CTask
{
public:
	typedef CTask inherited;

	CEndTask (const char *name):inherited (name,0){}
	virtual ~CEndTask(void) { }

	virtual int open (void *) { return 0; }
	virtual int open (void) { return 0; }

	virtual int put (ACE_Message_Block *message,ACE_Time_Value *timeout)
	{
		ACE_DEBUG ((LM_DEBUG, "(%P|%t) %s CEndTask::put() mb:%s\n", m_name,message->rd_ptr()));
		message->release ();
		return 0;
	}
};


#endif
