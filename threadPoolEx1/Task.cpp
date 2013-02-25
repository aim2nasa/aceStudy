#include "Task.h"
#include <iostream>

using namespace std;

Task::Task (int sub_tasks)
:barrier_(0)
{
	cout<<"Task constructor(0x"<<std::hex<<this<<")"<<endl;
}

Task::~Task (void)
{
    delete barrier_;
	cout<<"Task destructor(0x"<<std::hex<<this<<")"<<endl;
}

int Task::start (int threads)
{
	cout<<"Task start("<<threads<<")"<<endl;

    barrier_ = new ACE_Barrier(threads);
    return this->activate (THR_NEW_LWP, threads);
}

int Task::close (u_long flags)
{
	cout<<"Task close("<<flags<<")"<<endl;
    return 0;
}

int Task::svc (void)
{
	cout<<"Task svc start"<<endl;
	
	this->barrier_->wait ();

	cout<<"Task svc end"<<endl;
    return 0;
}
