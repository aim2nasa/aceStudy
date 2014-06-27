#ifndef __CFIRST_H__
#define __CFIRST_H__

#include "CBasicTask.h"

class CFirst : public CBasicTask {
protected:
	virtual int process(ACE_Message_Block *mb)
	{
		ACE_DEBUG ((LM_DEBUG,ACE_TEXT ("CFirst::process()\n")));
		return 0;
	}
};

#endif