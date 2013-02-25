#include <iostream>
#include "Task.h"

using namespace std;

int main(int argc, char *argv[])
{
	Task t;
	t.start(3);

	t.wait();
	cout<<"end of main"<<endl;
	return 0;
};