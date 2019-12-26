#ifndef _UAL_UDP_H
#define _UAL_UDP_H

#include "config.h"
#include "ual/helper/this_subclass.h"
#include <string>

using namespace std;

namespace UAL_NAMESPACE{

template<class Impl>
class udp:public __this_subclass<Impl>{
    using __this_subclass<Impl>::sub_this;
public:

    enum class ip_type{
        IPV4 = 0,
        IPV6
    };
public:
    /*
     *@brief bind want use udp transport the address and port
     */
    int common_bind(const string &ipaddr,const string port)
    {
        return sub_this->udp_common_bind(ipaddr,port);
    }

    int Client_session(const string & ipaddr, const string &port,function<void(string &flag)> session_call)
    {
        return sub_this->udp_Client_session(ipaddr, port,session_call);
    }

    int Server_context(const string & ipaddr,const string &port,function<void(const string &dst,string &src)> context_call)
    {
        return sub_this->udp_Server_context( ipaddr, port, context_call);
    }
  
    int request(string &data)
    {
        return sub_this->udp_request(data);
    }
   
    int response(string &ack)
    {
        return sub_this->udp_response( ack);
    }
   
    void close()
    {
        sub_this->udp_close();
    }

};


}


#endif