#ifndef _UAL_STREAM_H
#define _UAL_STREAM_H

#include "config.h"
#include "ual/helper/this_subclass.h"
#include "ual/executr.h"

namespace UAL_NAMESPACE{

template<class Impl>
class stream:public __this_subclass<Impl>{

    using __this_subclass<Impl>::sub_this;
public:
    typedef int backlog;
    void start(backlog num,function<void(void)> connection_call)
    {
        sub_this->start_init(num,connection_call);
    }

    void recv(size_t len,string &buf,function<void(string _buf,size_t _len)> recv_call)
    {
        sub_this->recv_data(len,buf,recv_call);
    }

    void send(string &buf,function<void(string _buf)> send_call)
    {
        sub_this->send_data(buf,send_call);
    }

    int close()
    {
        sub_this->clsoe_data();
    }
};


}