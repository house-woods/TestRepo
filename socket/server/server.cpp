#include <iostream>
#include "ace/SOCK_Acceptor.h"
#include "ace/SOCK_Stream.h"
//
using namespace std;
#define SIZE_DATA 18
#define SIZE_BUF 1024
#define NO_ITERATIONS 5

class Server
{
public:
	Server (int port): server_addr_(port),peer_acceptor_(server_addr_)
	{
		data_buf_= new char[SIZE_BUF];
	};
	//Handle the connection once it has been established. Here the
	//connection is handled by reading SIZE_DATA amount of data from the
	//remote and then closing the connection stream down.
	int handle_connection()
	{
		// Read data from client
		//for(int i=0;i<NO_ITERATIONS;i++)
                while(1)      
		{
			int byte_count=0;
			if( (byte_count=new_stream_.recv_n (data_buf_, SIZE_DATA, 0))==-1)
			{
				std::cout<<"Error in recv"<<endl;
			}
			else
			{
				data_buf_[byte_count]=0;
				std::cout<<"Server received:"<<data_buf_<<endl;
                                if('q' == data_buf_[0])
                                  break;             
			}
		}
		// Close new endpoint
	        if (new_stream_.close () == -1)
			std::cout<<"close"<<endl;
		return 0;
	};
	//Use the acceptor component peer_acceptor_ to accept the connection
	//into the underlying stream new_stream_. After the connection has been
	//established call the handle_connection() method.
	int accept_connections ()
	{
		if (peer_acceptor_.get_local_addr (server_addr_) == -1)
		{
			std::cout<<"Error in get_local_addr"<<endl;
		}
		std::cout<<"Starting server at port:"<<server_addr_.get_port_number()<<endl;

		// Performs the iterative server activities.
		while(1)
		{
			ACE_Time_Value timeout (ACE_DEFAULT_TIMEOUT);
			if (peer_acceptor_.accept (new_stream_, &client_addr_, &timeout)== -1)
			{
				std::cout<<"accept"<<endl;
				continue;
			}
			else
			{
				std::cout<<"Connection established with remote "<<client_addr_.get_host_name()<<":"<<client_addr_.get_port_number()<<endl;
				//Handle the connection
				handle_connection();
			}
		}
		return 0;
	};
private:
	char *data_buf_;
	ACE_INET_Addr server_addr_;
	ACE_INET_Addr client_addr_;
	ACE_SOCK_Acceptor peer_acceptor_;
	ACE_SOCK_Stream new_stream_;
};
int main (int argc, char *argv[])
{
	if(argc<2)
	{
		std::cout<<"Usage "<<argv[0]<<" <port_num>"<<endl;
		ACE_OS::exit(1);
	}
	Server server(ACE_OS::atoi(argv[1]));
	server.accept_connections();
}
