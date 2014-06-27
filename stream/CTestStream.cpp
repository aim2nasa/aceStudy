#include "CTestStream.h"
#include "CEnd.h"
#include "CFirst.h"
#include "CSecond.h"

CTestStream::CTestStream ()
:inherited()
{

}

int CTestStream::open(void *arg,Module *head, Module *tail)
{
	if (tail == 0) ACE_NEW_RETURN (tail,Module (ACE_TEXT ("end"),new CEnd()),-1);

	this->inherited::open (arg, head, tail);

	Module *firstModule;
	ACE_NEW_RETURN (firstModule,Module (ACE_TEXT ("first"),new CFirst()),-1);

	Module *secondModule;
	ACE_NEW_RETURN (secondModule,Module (ACE_TEXT ("second"),new CSecond()),-1);

	if(this->push (secondModule) == -1) ACE_ERROR_RETURN ((LM_ERROR,ACE_TEXT ("Failed to push %p\n"),ACE_TEXT ("second")),-1);
	if(this->push (firstModule) == -1) ACE_ERROR_RETURN ((LM_ERROR,ACE_TEXT ("Failed to push %p\n"),ACE_TEXT ("first")),-1);
	return 0;
}

int CTestStream::insert(const char* pBuffer,int nSize)
{
	ACE_Message_Block * mb;
	ACE_NEW_RETURN (mb, ACE_Message_Block (nSize),-1);
	mb->copy(reinterpret_cast<const char*>(pBuffer),nSize);
	if(this->put(mb->duplicate(), 0) == -1) {
		ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "put"), -1);
	}
	mb->release();
	return 0;
}