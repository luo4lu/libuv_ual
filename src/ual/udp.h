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
    int common_bind(const string &dst_ipaddr,const string &dst_port)
    {
        return sub_this->udp_common_bind(dst_ipaddr,dst_port);
    }

    int response_session(const string & src_ipaddr, const string &src_port,const string &data,function<void(const string &flag)> session_call)
    {
        return sub_this->udp_response_session(src_ipaddr, src_port,data,session_call);
    }

    int request_context(const string & src_ipaddr,const string &src_port,function<void(const string &dst,string &src)> context_call)
    {
        return sub_this->udp_request_context( src_ipaddr, src_port, context_call);
    }
  
    int get_data(string &data)
    {
        return sub_this->udp_get_data(data);
    }
   
    int response_data(string &ack)
    {
        return sub_this->udp_response_data( ack);
    }
   
    void close()
    {
        sub_this->udp_close();
    }

};


}


#endif