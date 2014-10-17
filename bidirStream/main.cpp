#include <iostream>
#include "CBiStream.h"

using namespace std;

int main(int argc, char *argv[])
{
	Que peer;
	CBiStream bs;
	if(bs.open(&peer,0)<0)
		ACE_ERROR_RETURN ((LM_ERROR,ACE_TEXT ("%p\n"),ACE_TEXT ("CBiStream.open()")),0);

	char buffer[128];
	for(int i=0;i<1;i++) {
		memset(buffer,0,sizeof(buffer));
		sprintf(buffer,"message %d",i+1);

		size_t nSize = strlen(buffer);
		ACE_Message_Block * mb;
		ACE_NEW_RETURN (mb, ACE_Message_Block(nSize),-1);
		mb->copy(reinterpret_cast<const char*>(buffer),nSize);

		printf("++++ [%d] put message: %s \n",i+1,buffer);

		if(bs.put(mb->duplicate()) == -1) ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "put"), -1);		//put
		mb->release();

		ACE_Message_Block * response;
		bs.get(response);

		printf("---- [%d] get message: %s \n",i+1,response->rd_ptr());
	}

	bs.close();
	cout<<"end of main"<<endl;
	return 0;
};