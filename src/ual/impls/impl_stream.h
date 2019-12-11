#ifndef _UAL_IMPL_STREAM_H
#define _UAL_IMPL_STREAM_H

#include "config.h"
#include "ual/stream.h"
#include <uv.h>
#include <iostream>

using namespace std;
namespace ual{

class libuv_stream: public stream<libuv_stream>{
public:
    /*
    *use tcp class to construct libuv_stream
    */
    template<class TcpImpl> 
    libuv_stream(const executor<TcpImpl> &tcp):
    _tstream(const_cast<uv_loop_t *>(&(static_cast<const TcpImpl*>(&tcp)->loop)))
    {};

    void read_data(size_t len,string &buf,function<void(string _buf,size_t _len)> recv_call);

    void send_data(string &buf,int nbuf, function<void(void)> send_call);

    int stopread_data();

public:
    function<void(string _buf,size_t _len)> _recv_call;
    function<void(void)> _send_call;
    uv_stream_t *_tstream;
    uv_write_t _wreq;
 //   uv_buf_t _tbuf;
};

static void alloc_cb(uv_handle_t *handle,size_t suggested_size,uv_buf_t *buf)
{
    auto data = uv_handle_get_data(handle);
    auto _str = static_cast<libuv_stream *>(data);
    /*buf->base = (char *)malloc(suggested_size);
    buf->len = suggested_size;
    uv_buf_init(buf->base,buf->len);*/
    _str->_recv_call(buf->base,buf->len);
}

static void read_cb(uv_stream_t *_hstream,ssize_t nread,const uv_buf_t *buf)
{
    auto data = uv_handle_get_data(reinterpret_cast<uv_handle_t *>(_hstream));
    auto _read = static_cast<libuv_stream *>(data);
    if(nread < 0)
        cout<<"read accept data error!!"<<endl;
    _read->_recv_call(buf->base,buf->len);
}

void libuv_stream::read_data(size_t len,string &buf,function<void(string _buf,size_t _len)> recv_call)
{
    uv_handle_set_data(reinterpret_cast<uv_handle_t*>(this->_tstream),this);
    this->_recv_call = recv_call;
    uv_buf_init(const_cast<char *>(buf.c_str()),buf.length());
    uv_read_start(this->_tstream,alloc_cb,read_cb);
}

static void send_cb(uv_write_t *req, int status)
{
    auto data = uv_handle_get_data(reinterpret_cast<uv_handle_t *>(req));
    auto _send = static_cast<libuv_stream *>(data);
    if(status != 0)
        cout<<"call write to char send fail!!"<<endl;
    _send->_send_call();
}

void libuv_stream::send_data(string &buf,int nbuf, function<void(void)> send_call)
{
    uv_handle_set_data(reinterpret_cast<uv_handle_t*>(&(this->_wreq)),this);
    this->_send_call = send_call;
    uv_buf_t _tbuf = uv_buf_init(const_cast<char *>(buf.c_str()),buf.length());
    uv_write(&(this->_wreq),this->_tstream,&_tbuf,nbuf,send_cb);
}

int libuv_stream::stopread_data()
{
    return uv_read_stop(this->_tstream);
}

}

#endif