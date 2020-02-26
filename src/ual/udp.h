#ifndef _UAL_UDP_H
#define _UAL_UDP_H

#include "config.h"
#include "ual/helper/this_subclass.h"
#include <string>

//用于非类成员函数使用类对象
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

    enum class request_type{
        POST = 0,
        GET
    };
public:
    /*
     *@brief bind want use udp transport the address and port
     *
     *@params dst_ipaddr:local IP address
     *@params dst_port:local port number
     *
     *@return succeed: 0     failed: -1 
     */
    int common_bind(const string &src_ipaddr,const string &src_port)
    {
        return sub_this->udp_common_bind(src_ipaddr,src_port);
    }

    /*
     *@brief Establishing a request context
     *
     * @params hostname:end on ip addr or hostname
     * @params api:end on piont number
     * @params data:send data of request resource
     * @params session_call:callback function 
     * 
     * @return succeed: 0     failed: -1 
     */
    int request_context(const string & dst_ip, const string &dst_port,const char *resource,request_type type,function<void(char * recv_data)> session_call)
    {
        return sub_this->udp_request_context(dst_ip, dst_port,resource,type,session_call);
    }

    /*
     *@brief Establishing a response session
     *
     * @params key:Identify the key in the session
     * @params value:Identify the value in the session
     * @params context_call:callback function 
     * 
     * @return succeed: 0     failed: -1 
     */
    int response_session(const char *key,request_type type,function<void(const char *src,char *dst)> context_call)
    {
        return sub_this->udp_response_session( key,type,context_call);
    }
    
    /*
     *@brief recv point get data and show data
     *
     * @params data:of response resource data
     * 
     * @return succeed: 0     failed: -1 
     */
    int show_data(char* data)
    {
        return sub_this->udp_show_data(data);
    }
    /*
     *@brief Pass the data that needs to be sent
     *
     * @params ack:response data 
     * 
     * @return succeed: 0     failed: -1 
     */
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