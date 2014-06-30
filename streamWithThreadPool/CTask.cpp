#include <ace/Message_Block.h>
#include "CTask.h"

CTask::CTask(const char * name,int numberOfThreads)
: m_nNumberOfThreads(numberOfThreads),m_barrier(numberOfThreads)
{
	ACE_OS::strcpy(m_name, name);
	ACE_DEBUG ((LM_DEBUG, "(%P|%t) %s CTask::Task(), threads:%d\n", m_name,m_nNumberOfThreads));
}

CTask::~CTask(void)
{
	ACE_DEBUG ((LM_DEBUG, "(%P|%t) %s CTask::~CTask(), threads:%d\n", m_name,m_nNumberOfThreads));
}

int CTask::open(void *arg)
{
	return this->activate(THR_NEW_LWP, m_nNumberOfThreads);
}

int CTask::put(ACE_Message_Block *message,
			  ACE_Time_Value *timeout)
{
	return this->putq(message, timeout);
}

int CTask::close(u_long flags)
{
	if (flags == 1) {
		ACE_Message_Block *hangupBlock = new ACE_Message_Block();
		hangupBlock->msg_type(ACE_Message_Block::MB_HANGUP);
		if (this->putq(hangupBlock->duplicate()) == -1) {
			ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "CTask::close() putq"), -1);
		}
		hangupBlock->release();
		return this->wait();
	} else {
		// This is where we can clean up any mess left over by each service thread.
		// In this CTask, there is nothing to do.
	}
	return 0;
}

int CTask::svc(void)
{
	m_barrier.wait();
	ACE_DEBUG ((LM_DEBUG, "(%P|%t) %s CTask::svc() barrier waited\n", m_name));

	ACE_Message_Block *messageBlock;
	while (1) {
		if ( this->getq(messageBlock, 0) == -1) {
			ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "CTask::svc() getq"), -1);
		}

		if (messageBlock->msg_type() == ACE_Message_Block::MB_HANGUP) {
			if (this->putq(messageBlock->duplicate()) == -1) {
				ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "CTask::svc() putq"), -1);
			}
			messageBlock->release();
			break;
		}

		ACE_DEBUG ((LM_DEBUG, "(%P|%t) %s CTask::svc(), mb:%s\n", m_name,messageBlock->rd_ptr()));

		ACE_OS::sleep (ACE_Time_Value (0, 250));

		if (put_next(messageBlock->duplicate()) == -1) {
			ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "CTask::svc() put_next"), -1);
		}
		messageBlock->release();
	}
	return 0;

}

const char * CTask::name(void) const
{
	return m_name;
}
