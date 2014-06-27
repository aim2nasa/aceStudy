#ifndef __CEND_H__
#define __CEND_H__

#include "CBasicTask.h"

class CEnd : public CBasicTask {
protected:
	virtual int process (ACE_Message_Block *mb)
	{
		ACE_DEBUG ((LM_DEBUG,ACE_TEXT ("CEnd::process()\n")));
		return 0;
	}
};

#endif