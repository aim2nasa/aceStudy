#include <iostream>
#include <ace/Log_Msg.h>
#include <ace/INET_Addr.h>
#include <ace/SOCK_Dgram.h>
#include <assert.h>

using namespace std;

int main(int argc, char *argv[])
{
	if(argc<2) {
		cout<<"Usage: udpReceiver port"<<endl;
		return -1;
	}

	cout<<"Receiver port is "<<atoi(argv[1])<<endl;

	ACE_INET_Addr my_addr (static_cast<u_short>(atoi(argv[1])));
	ACE_INET_Addr your_addr;
	ACE_SOCK_Dgram udp (my_addr);

	int i=0;
	char buff[6144];
	size_t buflen = sizeof (buff);
	while(1){
		ssize_t recv_cnt = udp.recv (buff, buflen, your_addr);
		cout<<"i="<<i++<<", "<<recv_cnt<<" bytes received"<<endl;
	}
	udp.close ();

	cout<<"end of main"<<endl;
	return 0;
};