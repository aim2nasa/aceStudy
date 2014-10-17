#include "CBiStream.h"
#include "Xmit.h"
#include "Recv.h"
#include "Crypt.h"
#include "Compressor.h"

CBiStream::CBiStream()
:m_pReader(0)
{

}

CBiStream::~CBiStream()
{

}

int CBiStream::open(void *arg,Module *head,Module *tail)
{
	Module* pXmitRecv = new Module("Xmit/Recv",new Xmit(),new Recv());
	if(this->push(pXmitRecv)==-1)
		ACE_ERROR_RETURN ((LM_ERROR,"%p\n","push(Xmit/Recv)"),-1);

	Module* pCrypt = new Module("Crypt",new Crypt(),new Crypt());
	if(this->push(pCrypt)==-1)
		ACE_ERROR_RETURN ((LM_ERROR,"%p\n","push(Crypt)"),-1);

	Module* pCompress = new Module("Compress",new Compressor(),new Compressor());
	if(this->push(pCompress)==-1)
		ACE_ERROR_RETURN ((LM_ERROR,"%p\n","push(Compress)"),-1);

	if(m_pReader) {
		Module* pReader= new Module("Reader",new Thru_Task(),m_pReader);
		if(this->push(pReader)==-1)
			ACE_ERROR_RETURN ((LM_ERROR,"%p\n","push(Reader)"),-1);
	}
	return 0;
}