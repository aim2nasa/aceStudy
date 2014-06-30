#ifndef __CFIRST_H__
#define __CFIRST_H__

#include "CBasicTask.h"
#include <assert.h>

class CFirst : public CBasicTask {
protected:
	virtual int process(ACE_Message_Block *mb)
	{
		const char *cp = mb->rd_ptr();
		assert(cp);

		printf("CFirst::process : %s",cp);
		return 0;
	}
};

#endif