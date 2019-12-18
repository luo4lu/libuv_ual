#ifndef _UAL_TCP_H
#define _UAL_TCP_H

#include "config.h"
#include "ual/helper/this_subclass.h"
#include "ual/executor.h"
#include "ual/stream.h"
#include <string>

using namespace std;

namespace UAL_NAMESPACE{

template<class Impl>
class tcp:public __this_subclass<Impl>{
    using __this_subclass<Impl>::sub_this;
public:
    enum class ip_type{
        IPV4 =0,
        IPV6
    };
    typedef int backlog;
public:
    /*
     *@brief bind connect the ipaddr and port 
     *
     * @params ipaddr:ip address to bind to
     * @params type:ip addr type,from enum ip_type choise
     * @params port:port address to bind to
     * 
     * @return succeed: 0     failed: other 
     */
    int bind(const string & ipaddr,ip_type type, int port)
    {
        return sub_this->tcp_bind(ipaddr,type, port);
    }

    /*
     *@brief client point send connect request 
     *
     * @params type:ip addr type,from enum ip_type choise
     * @params tcp_call:connect succeed beform runing callback function 
     * 
     * @return succeed: 0     failed: other 
     */
    int connect(ip_type type, function<void(void)> tcp_call)
    {
        return sub_this->tcp_connect(type,tcp_call);
    }

    /*
     *@brief server point listen from client the connect request 
     *
     * @params num:indicates the number of connections the kernel might queue
     * @params tcp_call:listen succeed beform runing callback function 
     * 
     * @return succeed: 0     failed: other 
     */
    int listen(backlog num,function<void(void)> connection_call)
    {
        return sub_this->listen_stream(num,connection_call);
    }

    /*
     *@brief server point recv from client point the connect request 
     *
     * @params strm:bind stream transport struct handle
     * 
     * @return succeed: 0     failed: other 
     */
    template<class StreamImpl>
    int accept(const stream<StreamImpl> &strm)
    {
        return sub_this->accept_data(strm);
    }

    /*
     *@brief close tcp connect 
     *
     * @params shutdown_cb:close tcp connect beform runing callback function 
     * 
     * @return succeed: 0     failed: other 
     */
    void shutdown(function<void(void)> shutdown_cb)
    {
        sub_this->tcp_shutdown(shutdown_cb);
    }

};

} // namespace UAL_NAMESPACE0

#endif