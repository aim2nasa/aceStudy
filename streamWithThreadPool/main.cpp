#include <iostream>
#include <ace/Stream.h>
#include "CTask.h"
#include "CEndTask.h"

using namespace std;

typedef ACE_Module<ACE_MT_SYNCH> Module;
typedef ACE_Stream<ACE_MT_SYNCH> Stream;

int main(int argc, char *argv[])
{
	Stream theStream;

	CTask *pTask1 = new CTask("Task 1", 1);
	CTask *pTask2 = new CTask("Task 2", 2*2);
//	CTask *pTask3 = new CTask("Task 3", 3*3);
	CEndTask *pTaskE = new CEndTask("End Task");

	Module *module1 = new Module("Module 1", pTask1);
	Module *module2 = new Module("Module 2", pTask2);
//	Module *module3 = new Module("Module 3", pTask3);
	Module *moduleEnd = new Module("End Module", pTaskE);

	if (theStream.push(moduleEnd) == -1) {
		ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "push"), -1);
	}

	//if (theStream.push(module3) == -1) {
	//	ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "push"), -1);
	//}

	if (theStream.push(module2) == -1) {
		ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "push"), -1);
	}

	if (theStream.push(module1) == -1) {
		ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "push"), -1);
	}

	for(int i=0;i<10;i++) {
		ACE_Message_Block *message = new ACE_Message_Block(128);
		ACE_OS::sprintf(message->wr_ptr(),"Message (%d)", i);

		if (theStream.put(message->duplicate(), 0) == -1) {
			ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "put"), -1);
		}
		message->release();
	}

	theStream.close();

	cout<<"end of main"<<endl;
	return 0;
};