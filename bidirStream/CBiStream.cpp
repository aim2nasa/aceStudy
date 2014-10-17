#include "CBiStream.h"
#include "Xmit.h"
#include "Recv.h"
#include "Crypt.h"
#include "Compressor.h"

CBiStream::CBiStream()
:m_pReader(0),recv_(0)
{

}

CBiStream::~CBiStream()
{

}

int CBiStream::open(Que* peer,Protocol_Task *reader)
{
	ACE_NEW_RETURN (recv_,Recv(peer),-1);

	Module* pXmitRecv = new Module("Xmit/Recv",new Xmit(peer),recv_);
	if(stream_.push(pXmitRecv)==-1)
		ACE_ERROR_RETURN ((LM_ERROR,"%p\n","push(Xmit/Recv)"),-1);

	Module* pCrypt = new Module("Crypt",new Crypt(),new Crypt());
	if(stream_.push(pCrypt)==-1)
		ACE_ERROR_RETURN ((LM_ERROR,"%p\n","push(Crypt)"),-1);

	Module* pCompress = new Module("Compress",new Compressor(),new Compressor());
	if(stream_.push(pCompress)==-1)
		ACE_ERROR_RETURN ((LM_ERROR,"%p\n","push(Compress)"),-1);

	if(reader) {
		Module* pReader= new Module("Reader",new Thru_Task(),m_pReader);
		if(stream_.push(pReader)==-1)
			ACE_ERROR_RETURN ((LM_ERROR,"%p\n","push(Reader)"),-1);
	}
	return 0;
}

int CBiStream::close (void)
{
	return stream_.close();
}

int CBiStream::put(ACE_Message_Block *mb,ACE_Time_Value *timeout)
{
	return stream_.put(mb,timeout);
}

int CBiStream::get(ACE_Message_Block *&mb,ACE_Time_Value *timeout)
{
	if(stream_.head()->reader()->msg_queue()->is_empty() && this->get() == -1)
		ACE_ERROR_RETURN ((LM_ERROR,"(%P|%t) Cannot get data into the stream.\n"),-1);

	return stream_.head()->reader()->getq(mb,timeout);
}

int CBiStream::get()
{
	// If there is no Recv module, we're in big trouble!
	if (recv_ == 0)
		ACE_ERROR_RETURN ((LM_ERROR,"(%P|%t) No Recv object!\n"),-1);

	// This tells the Recv module to go to it's peer() and read some
	// data.  Once read, that data will be pushed upstream.  If there is
	// a reader object then it will have a chance to process the data.
	// If not, the received data will be available in the message queue
	// of the stream head's reader object (eg --
	// stream().head()->reader()->msg_queue()) and can be read with our
	// other get() method below.
	if (recv_->get () == -1)
		ACE_ERROR_RETURN ((LM_ERROR,"(%P|%t) Cannot queue read request\n"),-1);

	// For flexibility I've added an error() method to tell us if
	// something bad has happened to the Recv object.
	if (recv_->error ())
		ACE_ERROR_RETURN ((LM_ERROR,"(%P|%t) Recv object error!\n"),-1);

	return 0;
}
