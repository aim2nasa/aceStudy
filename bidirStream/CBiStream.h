#ifndef __CBISTREAM_H__
#define __CBISTREAM_H__

#include "common.h"
#include "ace/Stream.h"

typedef ACE_Stream<ACE_MT_SYNCH> Stream;
typedef ACE_Module<ACE_MT_SYNCH> Module;
typedef ACE_Thru_Task<ACE_MT_SYNCH> Thru_Task;

class Protocol_Task;
class Recv;

//Bi-directional stream
class CBiStream{
public:
	typedef ACE_Stream<ACE_MT_SYNCH> inherited;

	CBiStream();
	~CBiStream();

	// Provide the stream with an ACE_SOCK_Stream on which it can
	// communicate.  If _reader is non-null, it will be added as the
	// reader task just below the stream head so that it can process
	// data read from the peer.
	int open(Que* peer,Protocol_Task *reader = 0);

	// Close the stream.  All of the tasks & modules will also be closed.
	int close(void);
	
	// putting data onto the stream will pass it through all protocol
	// levels and send it to the peer.
	int put(ACE_Message_Block *mb,ACE_Time_Value *timeout=0);

	// get will cause the Recv task (at the tail of the stream) to read
	// some data from the peer and pass it upstream.  The message block
	// is then taken from the stream reader task's message queue.
	int get(ACE_Message_Block *&mb,ACE_Time_Value *timeout=0);

	// Tell the Recv task to read some data and send it upstream.  The
	// data will pass through the protocol tasks and be queued into the
	// stream head reader task's message queue.  If you've installed a
	// _reader in open() then that task's recv() method will see the
	// message and may consume it instead of passing it to the stream
	// head for queueing.
	int get();
	
	Stream stream_;
	Protocol_Task* m_pReader;
	Recv *recv_;
};

#endif