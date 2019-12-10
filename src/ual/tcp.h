#ifndef _UAL_TCP_H
#define _UAL_TCP_H

#include "config.h"
#include "ual/helper/this_subclass.h"
#include "ual/executor.h"
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
    enum class point_mode{
        CLIENT = 0,
        SERVER
    };
    typedef int backlog;
public:
    int bind(const string & ipaddr,ip_type type, int port)
    {
        return sub_this->tcp_bind(ipaddr,type, port);
    }

    int connect(ip_type type, function<void(void)> tcp_call)
    {
        return sub_this->tcp_connect(type,tcp_call);
    }

    int listen(backlog num,function<void(void)> connection_call)
    {
        return sub_this->listen_stream(num,connection_call);
    }

    int accept()
    {
        return sub_this->accept_data();
    }

    void shutdown(function<void(void)> shutdown_cb)
    {
        sub_this->tcp_shutdown(shutdown_cb);
    }

};

} // namespace UAL_NAMESPACE0

#endif