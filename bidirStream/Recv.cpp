
// Recv.cpp,v 1.7 1999/09/22 03:13:51 jcej Exp

#include "Recv.h"
#include "ace/SOCK_Stream.h"

/* Construct the object with the peer reference and other appropriate
   initializations.
*/
Recv::Recv(Que* peer)
        : Protocol_Task(), peer_(peer), error_(0)
{
     // Create the tickler that get() will use to trigger recv()
     // through the baseclass.  Since we're single-threaded this is
     // probably overkill but it makes multi-threading easier if we
     // choose to do that.
    tickler_ = new ACE_Message_Block(1);
}

/* Be sure we manage the lifetime of the tickler to prevent a memory
   leak.
*/
Recv::~Recv(void)
{
    tickler_->release();
}

/* By putting the tickler to ourselves we cause things to happen in
   the baseclass that will invoke recv().  If we know we're single
   threaded we could directly call recv() and be done with it but then
   we'd have to do something else if we're multi-threaded.  Just let
   the baseclass worry about those things!
*/
int Recv::get(void)
{
    return this->put( tickler_, 0 );
}

int Recv::recv(ACE_Message_Block * message, ACE_Time_Value *timeout)
{
	peer_->dequeue(message);
	int nRtn = this->put_next(message->duplicate());
	message->release();

	return nRtn;
}
