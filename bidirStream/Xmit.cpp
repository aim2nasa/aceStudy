
// Xmit.cpp,v 1.5 1999/09/22 03:13:51 jcej Exp

#include "Xmit.h"
#include "ace/SOCK_Stream.h"

/* Construct the object with the peer connection and choose not to
   activate ourselves into a dedicated thread.  You might get some
   performance gain by activating but if you really want a
   multi-threaded apprroach you should handle that as a separate
   issue.  Attempting to force threading at this level will likely
   cause more trouble than you want to deal with.
*/
Xmit::Xmit(Que* peer)
        : Protocol_Task(),peer_(peer)
{
}

Xmit::~Xmit(void)
{
}

/* Check to see if we're being closed by the stream (flags != 0) or if
   we're responding to the exit of our svc() method.
*/
int Xmit::close(u_long flags)
{
     // Take care of the baseclass closure.
    int rval = inherited::close(flags);

    return( rval );
}

/* Our overload of send() will take care of sending the data to the
   peer.
*/
int Xmit::send(ACE_Message_Block *message, ACE_Time_Value *timeout)
{
	char buffer[256];
	sprintf_s(buffer,"%s",message->rd_ptr());
	buffer[message->size()]=0;
	printf("Xmit::send(%d) start,Input:%s\n",message->size(),buffer);

	int nSize = (int)message->size();

	ACE_Message_Block * mb;
	ACE_NEW_RETURN (mb, ACE_Message_Block(nSize),-1);
	mb->copy(reinterpret_cast<const char*>(buffer),nSize);
	peer_->enqueue(mb->duplicate());
	mb->release();

	printf("Xmit::send() %s (%d) bytes sent\n",buffer,nSize);
	message->release();

	printf("Xmit::send() end\n");
    return nSize;
}
