#ifndef __CTESTSTREAM_H__
#define __CTESTSTREAM_H__

#include "ace/Stream.h"

class CTestStream : public ACE_Stream<ACE_MT_SYNCH>
{
public:
	typedef ACE_Stream<ACE_MT_SYNCH> inherited;
	typedef ACE_Module<ACE_MT_SYNCH> Module;

	CTestStream ();

	virtual int open (void *arg,Module *head = 0, Module *tail = 0);
	int insert(const char* pBuffer,int nSize);
};

#endif