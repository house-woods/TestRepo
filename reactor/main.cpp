#include<ace/Reactor.h>
#include<ace/SOCK_Acceptor.h>
//#include <iostream>
//using namespace std;

#define PORT_NO 19998
typedef ACE_SOCK_Acceptor Acceptor;
class My_Accept_Hanler;

class My_Input_Handler:public ACE_Event_Handler
{
public:
    My_Input_Handler()
    {
        ACE_DEBUG((LM_DEBUG,"Constructor\n"));
    }
    int handle_input(ACE_HANDLE)
    {
        //peer_.recv_n(data,12);
        peer_.recv(data,12);
        ACE_DEBUG((LM_DEBUG,"%s\n",data));
        int rcvBuff;
        int rcvBuffSize = sizeof(rcvBuff);  
        int sockopt_ret = ACE_OS::getsockopt (this->get_handle(), SOL_SOCKET, SO_RCVBUF,
                                            (char *)&rcvBuff, &rcvBuffSize);
        ACE_DEBUG((LM_DEBUG,"Rcv Buff size:%d\n",rcvBuff));
        int sndBuff;
        int sndBuffSize = sizeof(sndBuff);  
        sockopt_ret = ACE_OS::getsockopt (this->get_handle(), SOL_SOCKET, SO_SNDBUF,
                                            (char *)&sndBuff, &sndBuffSize);
        ACE_DEBUG((LM_DEBUG,"Snd Buff size:%d\n",sndBuff));

        return 0;
    }
    ACE_HANDLE get_handle() const
    {
        return peer_.get_handle();
    }
    ACE_SOCK_Stream& peer_i()
    {
        return this->peer_;
    }
private:
    ACE_SOCK_Stream peer_;
    char data[12];
};

class My_Accept_Handler:public ACE_Event_Handler
{
public:
    My_Accept_Handler(ACE_Addr& addr)
    {
        this->open(addr);
    }
    int open(ACE_Addr& addr)
    {
        peer_acceptor.open(addr);
        return 0;
    }
    int handle_input(ACE_HANDLE handle)
    {
        My_Input_Handler* eh=new My_Input_Handler();
        if(this->peer_acceptor.accept(eh->peer_i(),0,0,1)==-1)
            ACE_DEBUG((LM_ERROR,"Error in connection\n"));
        ACE_DEBUG((LM_DEBUG,"Connection established\n"));
        ACE_Reactor::instance()->register_handler(eh,ACE_Event_Handler::READ_MASK);
        return -1;
    }
    ACE_HANDLE get_handle() const
    {
        return peer_acceptor.get_handle();
    }
private:
    Acceptor peer_acceptor;
};

int main(int argc,char* argv[])
{
    ACE_INET_Addr addr(PORT_NO);
    My_Accept_Handler* eh=new My_Accept_Handler(addr);
    ACE_Reactor::instance()->register_handler(eh,ACE_Event_Handler::ACCEPT_MASK);
    while(1)
        ACE_Reactor::instance()->handle_events();
    return 0;
}
