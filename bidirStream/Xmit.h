// Xmit.h,v 1.4 1999/02/01 00:19:30 schmidt Exp

#ifndef XMIT_H
#define XMIT_h

#include "Protocol_Task.h"

// Forward reference reduces #include dependencies
class ACE_SOCK_Stream;

/* A class suitable for sending data to a peer from within an
   ACE_Stream.
 */
class Xmit : public Protocol_Task
{
public:
  typedef Protocol_Task inherited;

  // We must be given a valid peer when constructed.  Without that we
  // don't know who to send data to.
  Xmit (Que* peer);
  ~Xmit (void);

  // As you know, close() will be called in a couple of ways by the
  // ACE framework.  We use that opportunity to terminate the
  // connection to the peer.
  int close (u_long flags);

protected:
	Que* peer_;

  // Send the data to the peer.  By now it will have been completely
  // protocol-ized by other tasks in the stream.
  int send (ACE_Message_Block *message,
            ACE_Time_Value *timeout);
};

#endif /* XMIT_H */
