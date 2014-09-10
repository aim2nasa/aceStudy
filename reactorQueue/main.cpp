#include <iostream>
#include <ace/Reactor.h>
#include <ace/Task.h>
#include <ace/Auto_Event.h>
#include <ace/Reactor_Notification_Strategy.h>
#include <conio.h>
#include <assert.h>

using namespace std;

class Message_Handler : public ACE_Task <ACE_SYNCH>
{
public:
	Message_Handler (ACE_Reactor &reactor);

	virtual int handle_input (ACE_HANDLE h);
	// Called back within the context of the <ACE_Reactor> Singleton to
	// dequeue and process the message on the <ACE_Message_Queue>.

	virtual int svc (void);
	// Run the "event-loop" periodically putting messages to our
	// internal <Message_Queue> that we inherit from <ACE_Task>.

private:
	ACE_Reactor_Notification_Strategy notification_strategy_;
	// This strategy will notify the <ACE_Reactor> Singleton when a new
	// message is enqueued.
};

Message_Handler::Message_Handler (ACE_Reactor &reactor)
: notification_strategy_ (&reactor,
						  this,
						  ACE_Event_Handler::READ_MASK)
{
	this->reactor (&reactor);

	// Set this to the Reactor notification strategy.
	this->msg_queue ()->notification_strategy (&this->notification_strategy_);

	//if (this->activate ()) ACE_ERROR ((LM_ERROR,"%p\n","activate"));
}

int Message_Handler::svc (void)
{
	//for (int i = 0;; i++)
	//{
	//	ACE_Message_Block *mb = 0;

	//	ACE_NEW_RETURN (mb,
	//		ACE_Message_Block (1),
	//		0);

	//	mb->msg_priority (i);

	//	// Note that this putq() call with automagically invoke the
	//	// notify() hook of our ACE_Reactor_Notification_Strategy,
	//	// thereby informing the <ACE_Reactor> Singleton to call our
	//	// <handle_input> method.
	//	if (this->putq (mb) == -1)
	//	{
	//		if (errno == ESHUTDOWN)
	//			ACE_ERROR_RETURN ((LM_ERROR,
	//			"(%t) queue is deactivated"), 0);
	//		else
	//			ACE_ERROR_RETURN ((LM_ERROR,
	//			"(%t) %p\n",
	//			"putq"),
	//			-1);
	//	}
	//}

	return 0;
}

int Message_Handler::handle_input (ACE_HANDLE h)
{
//	ACE_DEBUG ((LM_DEBUG,"(%t) Message_Handler::handle_input(%d)\n",h));
	assert(this->msg_queue()->message_count()==1);

	ACE_Message_Block *pBlock = 0;
	this->getq(pBlock);

	cout<<"processing :"<<pBlock->rd_ptr()<<endl;

	pBlock->release ();
	return 0;
}

class CInpWait  : public ACE_Task <ACE_MT_SYNCH>
{
public:
	CInpWait(ACE_Task<ACE_SYNCH>* pTask):m_pTask(pTask)
	{
	}

	virtual int svc()
	{
		char tmp[256];
		cout<<"Input string and then enter"<<endl;
		while(true){
			if(gets(tmp)) {
				if(strncmp(tmp,"q",1)==0) {
					cout<<"quit"<<endl;
					break;
				}else{
					tmp[strlen(tmp)]=0;
					ACE_Message_Block* pBlock = new ACE_Message_Block(strlen(tmp)+1);
					pBlock->copy(reinterpret_cast<const char*>(tmp),strlen(tmp)+1);
					m_pTask->putq(pBlock);
				}
			}
		}
		return 0;
	}
	
	ACE_Task<ACE_SYNCH>* m_pTask;
};

int main(int argc, char *argv[])
{
	ACE_Reactor reactor;

	Message_Handler handler(reactor);

	CInpWait inp(&handler);
	inp.activate(THR_NEW_LWP,1);

	int result = 0;
	while (result != -1)
		result = reactor.handle_events ();

	cout<<"end of main"<<endl;
	return 0;
};