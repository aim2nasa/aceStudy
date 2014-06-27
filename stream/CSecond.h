#ifndef __CSECOND_H__
#define __CSECOND_H__

#include "CBasicTask.h"

class CSecond : public CBasicTask {
protected:
	virtual int process (ACE_Message_Block *mb)
	{
		ACE_DEBUG ((LM_DEBUG,ACE_TEXT ("CSecond::proscess()\n")));
		return 0;
	}
};

#endif