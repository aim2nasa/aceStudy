#include "CBasicTask.h"

int CBasicTask::open(void * unused)
{
	ACE_UNUSED_ARG (unused);
	return this->activate(THR_NEW_LWP,1);
}

int CBasicTask::svc(void)
{
	for (ACE_Message_Block *message = 0; ; )
	{
		if (this->getq (message) == -1) ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"),ACE_TEXT ("getq")),-1);

		if (message->msg_type () == ACE_Message_Block::MB_HANGUP)
		{
			if (this->putq (message) == -1)
			{
				ACE_ERROR ((LM_ERROR,ACE_TEXT ("%p\n"),ACE_TEXT ("Task::svc() putq")));
				message->release();
			}
			break;
		}

		if (this->process(message) == -1)
		{
			message->release ();
			ACE_ERROR_RETURN ((LM_ERROR,ACE_TEXT ("%p\n"),ACE_TEXT ("process")),-1);
		}

		if (put_next(message->duplicate()) == -1) {
			ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "Task::svc() put_next"), -1);
		}

		message->release();
	}
	return 0;
}

int CBasicTask::close(u_long flags)
{
  int rval = 0;

  if (flags == 1)
  {
	  ACE_Message_Block *hangup = new ACE_Message_Block ();
	  hangup->msg_type (ACE_Message_Block::MB_HANGUP);
	  if (this->putq (hangup) == -1)
	  {
		  hangup->release ();
		  ACE_ERROR_RETURN ((LM_ERROR,ACE_TEXT ("%p\n"),ACE_TEXT ("Task::close() putq")),-1);
	  }
	  rval = this->wait();
  }
  return rval;
}

int CBasicTask::put(ACE_Message_Block *message,ACE_Time_Value *timeout)
{
	return this->putq(message, timeout);
}