#ifndef _UAL_TCP_H
#define _UAL_TCP_H

#include "config.h"
#include "ual/helper/this_subclass.h"
#include "ual/executr.h"
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
public:
    int bind(const string & ipaddr,ip_type type, int port)
    {
        return sub_this->tcp_bind(ipaddr,type, port);
    }

    int connect(function<void(void)> tcp_call)
    {
        return sub_this->tcp_connect(tcp_call);
    }

    int listen()
    {
        return sub_this->tcp_listen();
    }

    void abort(string &ipaddr)
    {
        sub_this->tcp_abort(ipaddr);
    }

}

} // namespace UAL_NAMESPACE0