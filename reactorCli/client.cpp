#include <iostream>
#include "ace/SOCK_Connector.h" 
#include "ace/INET_Addr.h" 
#include "ace/Log_Msg.h" 
#include "ace/OS_NS_stdio.h" 
#include "ace/OS_NS_string.h" 
#include "ace/OS_NS_unistd.h"

#define SIZE_BUF 256

static char* SERVER_HOST = "127.0.0.1";
static u_short SERVER_PORT = 19001;

int main(int argc, char *argv[])
{
	ACE_SOCK_Stream client_stream;
	ACE_INET_Addr remote_addr(SERVER_PORT,SERVER_HOST);
	ACE_SOCK_Connector connector;

	ACE_DEBUG((LM_DEBUG, "(%P|%t) Starting connect to %s: %d \n",remote_addr.get_host_name(), remote_addr.get_port_number()));
	if (connector.connect(client_stream, remote_addr) == -1)
		ACE_ERROR_RETURN((LM_ERROR, "(%P|%t) %p \n", "connection failed"), -1);
	else
		ACE_DEBUG((LM_DEBUG, "(%P|%t) connected to %s \n",remote_addr.get_host_name()));

	int nRtn = 0;
	char buffer[SIZE_BUF];
	while (true){
		std::cout << ": ";
		std::cin >> buffer;

		if ((nRtn=client_stream.send_n(buffer, SIZE_BUF)) == -1)
			ACE_DEBUG((LM_DEBUG, "(%P|%t) Error send_n(%d)\n", nRtn));

		// recv
		char recv_buff[SIZE_BUF] = { 0 };
		if ((nRtn=client_stream.recv_n(recv_buff, sizeof(recv_buff))) == -1)
			ACE_ERROR((LM_ERROR, "(%P|%t) Error recv_n(%d)\n",nRtn));
		else
			ACE_DEBUG((LM_DEBUG, "(%P|%t) %dbytes received:%s\n", nRtn,recv_buff));
	}

	ACE_OS::sleep(1);

	if (client_stream.close() == -1)
		ACE_ERROR_RETURN((LM_ERROR, "(%P|%t) %p \n", "close"), -1);

	return 0;
}