#ifndef __CSECOND_H__
#define __CSECOND_H__

#include "CBasicTask.h"
#include <assert.h>

class CSecond : public CBasicTask {
protected:
	virtual int process (ACE_Message_Block *mb)
	{
		const char *cp = mb->rd_ptr();
		assert(cp);

		printf("CSecond::process : %s",cp);
		return 0;
	}
};

#endif