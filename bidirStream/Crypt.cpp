
// Crypt.cpp,v 1.7 2000/04/09 18:24:24 jcej Exp

#include "Crypt.h"
#include "ace/SOCK_Stream.h"

/* The expected constructor...
 */
Crypt::Crypt( void )
        : Protocol_Task()
{
}

Crypt::~Crypt(void)
{
}

/* To send the data we'll apply a signature and encryption.
 */
int Crypt::send(ACE_Message_Block *message, ACE_Time_Value *timeout)
{
	char buffer[256];
	sprintf_s(buffer,"%s",message->rd_ptr());
	buffer[message->size()]=0;
	printf("Crypt::send(%d) start,Input:%s\n",message->size(),buffer);

	ACE_Message_Block * encrypted = new ACE_Message_Block(message->size()+2);
	ACE_OS::sprintf(encrypted->wr_ptr(),"[%s",buffer);

	sprintf_s(buffer,"%s",encrypted->rd_ptr());
	buffer[encrypted->size()]=0;
	printf("Crypt::send %s\n",buffer);

    this->put_next( encrypted );

    message->release();
	printf("Crypt::send() end\n");
    return( 0 );
}

/* The upstream movement requires that we decrypt what the peer has
   given us.
*/
int Crypt::recv(ACE_Message_Block *message, ACE_Time_Value *timeout)
{
	char buffer[256];
	sprintf_s(buffer,"%s",message->rd_ptr());
	buffer[message->size()]=0;
	printf("Crypt::recv(%d) start,Input:%s\n",message->size(),buffer);

	ACE_Message_Block * decrypted = new ACE_Message_Block(message->size()-1);
	ACE_OS::sprintf(decrypted->wr_ptr(),"%s",buffer+1);

	sprintf_s(buffer,"%s",decrypted->rd_ptr());
	buffer[decrypted->size()]=0;
	printf("Crypt::recv %s\n",buffer);

	this->put_next( decrypted );

	message->release();
	printf("Crypt::recv() end\n");
    return( 0 );
}
