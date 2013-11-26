#include <iostream>
#include <ace/Task.h>
#include <ace/Auto_Event.h>
#include <ace/Manual_Event.h>

using namespace std;

ACE_Auto_Event		aEvt;
ACE_Manual_Event	mEvt;

class CTask : public ACE_Task < ACE_MT_SYNCH >
{
public:
	CTask (){}
	~CTask (){}

	int svc (void)
	{
		for(int i=0;i<3;i++) {
			Sleep(3000);
			aEvt.signal();
			mEvt.signal();
		}
		return 0;
	}
};

int main(int argc, char *argv[])
{
	CTask t;
	t.activate();

	for(int i=0;i<3;i++) {
		cout<<"before manualEvent Wait"<<endl;
		mEvt.wait();
		cout<<"after manualEvent Wait"<<endl;
	}

	for(int i=0;i<3;i++) {
		cout<<"before autoEvent Wait"<<endl;
		aEvt.wait();
		cout<<"after autoEvent Wait"<<endl;
	}

	cout<<"end of main"<<endl;
	return 0;
};