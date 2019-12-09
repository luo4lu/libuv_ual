#ifndef _UAL_IMPL_STREAM_H
#define _UAL_IMPL_STREAM_H

#include "config.h"
#include "ual/tcp.h"
#include <uv.h>
#include <iostream>

namespace ual{

class libuv_stream:public stream<libuv_stream>{

    libuv_stream(){};

    void read_data(size_t len,string &buf,function<void(string _buf,size_t _len)> recv_call);

    void send_data(string &buf,function<void(string _buf)> send_call);

    int close_data();

public:
    function<void(void)> _connection_call;
    uv_stream_t _t_stream;
};

void libuv_stream::read_data(size_t len,string &buf,function<void(string _buf,size_t _len)> recv_call)
{

}

void libuv_stream::send_data(string &buf,function<void(string _buf)> send_call)
{

}

int libuv_stream::stopread_data();
{

}

}