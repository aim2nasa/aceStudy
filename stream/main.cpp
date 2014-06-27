#include <iostream>
#include "CTestStream.h"

using namespace std;

int main(int argc, char *argv[])
{
	CTestStream ts;

	char buffer[128];

	if(ts.open(0)<0)
		ACE_ERROR_RETURN ((LM_ERROR,ACE_TEXT ("%p\n"),ACE_TEXT ("CTestStream.open()")),0);

	for(int i=0;i<1;i++) {
		sprintf(buffer,"message %d\n",i+1);
		ts.insert(buffer,strlen(buffer));
	}

	Sleep(3000);
	ts.close();

	cout<<"end of main"<<endl;
	return 0;
};