#include <iostream>
#include "ace/SOCK_Connector.h" 
#include "ace/INET_Addr.h" 
#include "ace/Log_Msg.h" 
#include "ace/OS_NS_stdio.h" 
#include "ace/OS_NS_string.h" 
#include "ace/OS_NS_unistd.h"

#define SIZE_BUF 256

class Client
{
public:
	Client(char *hostname, int port) :remote_addr_(port, hostname)
	{
		data_buf_ = new char[SIZE_BUF];
	}

	int connect_to_server();
	int send_to_server();
	int close();

private:
	ACE_SOCK_Stream client_stream_;
	ACE_INET_Addr remote_addr_;
	ACE_SOCK_Connector connector_;

	char *data_buf_;
};

int
Client::connect_to_server()
{
	ACE_DEBUG((LM_DEBUG, "(%P|%t) Starting connect to %s: %d \n",
		remote_addr_.get_host_name(), remote_addr_.get_port_number()));

	if (connector_.connect(client_stream_, remote_addr_) == -1)
	{
		ACE_ERROR_RETURN((LM_ERROR, "(%P|%t) %p \n", "connection failed"), -1);
	}
	else
	{
		ACE_DEBUG((LM_DEBUG, "(%P|%t) connected to %s \n",
			remote_addr_.get_host_name()));
	}

	return 0;
}

int
Client::send_to_server()
{
	while (true){
		std::cout << ": ";
		std::cin >> data_buf_;

		int n = 0;
		n = client_stream_.send_n(data_buf_, SIZE_BUF);
		if (n == -1) {
			ACE_ERROR_RETURN((LM_ERROR, "(%P|%t) %p \n", "send_n"), 0);
		}

		// recv
		char recv_buff[SIZE_BUF] = { 0 };
		n = client_stream_.recv_n(recv_buff, sizeof(recv_buff));
		if (n == -1) {
			ACE_ERROR((LM_ERROR, "%p \n", "Error in recv"));
		}
		else {
			ACE_DEBUG((LM_DEBUG, "Client received: %s \n", recv_buff));
		}
	}
	return 0;
}

int
Client::close()
{
	if (client_stream_.close() == -1)
		ACE_ERROR_RETURN((LM_ERROR, "(%P|%t) %p \n", "close"), -1);
	else
		return 0;
}

static char* SERVER_HOST = "127.0.0.1";
static u_short SERVER_PORT = 19001;

int main(int argc, char *argv[])
{
	Client client(SERVER_HOST, SERVER_PORT);
	client.connect_to_server();
	client.send_to_server();
	ACE_OS::sleep(1);
	client.close();

	return 0;
}