#ifndef __CEND_H__
#define __CEND_H__

#include "CBasicTask.h"
#include <assert.h>

class CEnd : public CBasicTask {
protected:
	virtual int process (ACE_Message_Block *mb)
	{
		const char *cp = mb->rd_ptr();
		assert(cp);

		printf("CEnd::process : %s",cp);
		return 0;
	}

	virtual int put (ACE_Message_Block *message,ACE_Time_Value *timeout)
	{
		const char *cp = message->rd_ptr();
		assert(cp);
		printf("CEnd::put() %s",cp);
		message->release();
		return 0;
	}
};

#endif