#ifndef __CBASICTASK_H__
#define __CBASICTASK_H__

#include "ace/Stream.h"

class CBasicTask : public ACE_Task<ACE_MT_SYNCH>
{
public:
	typedef ACE_Task<ACE_MT_SYNCH> inherited;

	CBasicTask ():inherited(){}

	virtual int open(void * = 0);
	virtual int svc(void);
	virtual int close(u_long flags);
	int put(ACE_Message_Block *message,ACE_Time_Value *timeout);

protected:
	virtual int process (ACE_Message_Block *message_block) = 0;
};

#endif