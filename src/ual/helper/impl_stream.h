#ifndef _UAL_IMPL_STREAM_H
#define _UAL_IMPL_STREAM_H

#include "config.h"
#include "ual/tcp.h"
#include <uv.h>
#include <iostream>

namespace ual{

class libuv_stream:public stream<libuv_stream>{

    libuv_stream(){};

    void start_init(backlog num,function<void(void)> connection_call);

    void recv_data(size_t len,string &buf,function<void(string _buf,size_t _len)> recv_call);

    void send_data(string &buf,function<void(string _buf)> send_call);

    int close_data();

public:
    function<void(void)> _connection_call;
    uv_stream_t _t_stream;
};

static void connect_cb(uv_stream_t *connect_h)
{
    auto data = uv_handle_get_data(reinterpret_cast<uv_tcp_t*>(connect_h));
    auto net = static_cast<libuv_stream *>(data);
    net->_connection_call();
}

void libuv_stream::start_init(backlog num,function<void(void)> connection_call)
{
    uv_listen(_t_stream,num,connect_cb);
}


}