#include <iostream>
#include <ace/Reactor.h>
#include <ace/Task.h>
#include <ace/Auto_Event.h>
#include <conio.h>

using namespace std;

class CMyHandler : public ACE_Event_Handler
{
public:
	CMyHandler(ACE_Reactor &reactor);

	int reg(ACE_HANDLE event_handle);
	int handle_signal(int signum, siginfo_t * = 0, ucontext_t * = 0);
	int handle_close(ACE_HANDLE handle,ACE_Reactor_Mask close_mask);
};

CMyHandler::CMyHandler (ACE_Reactor &reactor)
{
	this->reactor (&reactor);
}

int CMyHandler::reg(ACE_HANDLE event_handle)
{
	return this->reactor()->register_handler (this,event_handle);
}

int CMyHandler::handle_signal (int, siginfo_t *, ucontext_t *)
{
	//m_evt가 signal된 것은 종료('q')를 의미하므로 이벤트 루프를 종료한다
	this->reactor()->close();
	return 0;
}

int CMyHandler::handle_close (ACE_HANDLE,
							 ACE_Reactor_Mask)
{
	return 0;
}

class CInpWait  : public ACE_Task <ACE_MT_SYNCH>
{
public:

	virtual int svc()
	{
		while(true){
			if(_kbhit())
				if(_getch()=='q')
					break;
		}
		m_evt.signal();
		return 0;
	}

	ACE_Auto_Event m_evt;
};

int main(int argc, char *argv[])
{
	ACE_Reactor reactor;
	CMyHandler handler(reactor);

	CInpWait inp;
	inp.activate(THR_NEW_LWP,1);

	handler.reg(inp.m_evt.handle());

	int result = 0;
	while (result != -1)
		result = reactor.handle_events ();

	cout<<"end of main"<<endl;
	return 0;
};