
// Compressor.cpp,v 1.7 2000/04/09 18:24:24 jcej Exp

#include "Compressor.h"
#include "ace/SOCK_Stream.h"

Compressor::Compressor( void )
        : Protocol_Task()
{
    ;
}

Compressor::~Compressor(void)
{
    ;
}

/* This is where you insert your compression code.  Most compressors
   want to work on a block of data instead of a byte-stream.
   Fortunately the message block has a block that can be compressed.
   Take a look at libz for a quick way to add compression to your
   apps
 */
int Compressor::send(ACE_Message_Block *message, ACE_Time_Value *timeout)
{
	char buffer[256];
	sprintf_s(buffer,"%s",message->rd_ptr());
	buffer[message->size()]=0;
	printf("Compressor::send(%d) start,Input:%s\n",message->size(),buffer);

	ACE_Message_Block * compressed = new ACE_Message_Block(message->size()+2);
	ACE_OS::sprintf(compressed->wr_ptr(),"<%s",buffer);

	sprintf_s(buffer,"%s",compressed->rd_ptr());
	buffer[compressed->size()]=0;
	printf("Compressor::%s\n",buffer);

    this->put_next(compressed->duplicate());

    compressed->release();
	printf("Compressor::send() end\n");
    return( 0 );
}

/* And here's the decompression side.  We've written Xmit/Recv so that
   we're guaranteed to get an entire block of compressed data.  If
   we'd used recv() in the Recv object then we might have gotten a
   partial block and that may not decompress very nicely.
 */
int Compressor::recv(ACE_Message_Block *message, ACE_Time_Value *timeout)
{
	char buffer[256];
	sprintf_s(buffer,"%s",message->rd_ptr());
	buffer[message->size()]=0;
	printf("Compressor::recv(%d) start,Input:%s\n",message->size(),buffer);

	ACE_Message_Block * uncompress = new ACE_Message_Block(message->size()-1);
	ACE_OS::sprintf(uncompress->wr_ptr(),"%s",buffer+1);

	sprintf_s(buffer,"%s",uncompress->rd_ptr());
	buffer[uncompress->size()]=0;
	printf("Compressor::recv %s\n",buffer);

	this->put_next( uncompress );

	message->release();
	printf("Compressor::recv() end\n");
	return( 0 );
}

