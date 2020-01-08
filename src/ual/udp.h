#ifndef _UAL_UDP_H
#define _UAL_UDP_H

#include "config.h"
#include "ual/helper/this_subclass.h"
#include <string>

#define CONTAINING_RECORD(addr,type,field) ((type*)((unsigned char*)addr - (unsigned long)&((type*)0)->field))
//　　addr:  结构体中某个成员变量的地址
//　　type:  结构体的原型
//　　field: 结构体的某个成员(与前面相同)

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

    int request_context(const string & hostname, const string &api,const char *data,function<void(char * flag)> session_call)
    {
        return sub_this->udp_request_context(hostname, api,data,session_call);
    }

    int response_session(const char *key, const char *value,function<void(const char *src,char *dst)> context_call)
    {
        return sub_this->udp_response_session( key,value,context_call);
    }
  
    int get_data(char* data)
    {
        return sub_this->udp_get_data(data);
    }
   
    int response_data(char *ack)
    {
        return sub_this->udp_response_data( ack);
    }
   
    void close()
    {
        sub_this->udp_close();
    }
    /*
     *@brief set timeout(send or receive before set this value)
     *
     * @params sec:set timeout second 
     * 
     * @return succeed: 0     failed: -1 
     */
    int set_timeout(unsigned int sec)
    {
        return sub_this->udp_set_timeout(sec);
    }
};


}


#endif