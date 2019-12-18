#ifndef _UAL_IMPL_STREAM_H
#define _UAL_IMPL_STREAM_H

#include "config.h"
#include "ual/stream.h"
#include <uv.h>
#include <iostream>
#include <string.h>

using namespace std;
namespace ual{

class libuv_stream: public stream<libuv_stream>{
public:
    
    /*libuv_stream(){
    }*/

    int recv_data( function<void(const string &_buf,size_t _len)> recv_call);

    int send_data(const string &buf,int nbuf, function<void(void)> send_call);

    int stoprecv_data();

    friend class libuv_tcp;
public:
    function<void(const string &_buf,size_t _len)> _recv_call;
    function<void(void)> _send_call;
    string _result;
    uv_stream_t _tstream;
    uv_stream_t _Ctstream;
    uv_write_t _wreq;
};

static void alloc_cb(uv_handle_t *handle,size_t suggested_size,uv_buf_t *buf)
{
    buf->base = new char[suggested_size];
    if(buf->base == NULL)
    {
        cout<<"malloc fail~~~"<<endl;
        return ;
    }
    buf->len = suggested_size;
}

static void recv_cb(uv_stream_t *_hstream,ssize_t nread,const uv_buf_t *buf)
{
    auto data = uv_handle_get_data(reinterpret_cast<uv_handle_t*>(_hstream));
    auto _read = reinterpret_cast<libuv_stream *>(data);
    if(nread <= 0)
    {
        delete(buf->base);
        uv_close((uv_handle_t *)_hstream,NULL);
        cout<<"read accept data error!! :"<<uv_err_name(nread)<<endl;
    }
    uv_buf_init(buf->base,nread);
    _read->_recv_call(buf->base,nread); 
}

int libuv_stream::recv_data(function<void(const string &_buf,size_t _len)> recv_call)
{
    uv_handle_set_data(reinterpret_cast<uv_handle_t*>(&(this->_tstream)),this);
    this->_recv_call = recv_call;
    int value = uv_read_start(&(this->_tstream),alloc_cb,recv_cb);
    if(value != 0)
        cout<<uv_strerror(value)<<endl;
    return value;   
}

static void send_cb(uv_write_t *req, int status)
{
    auto data = uv_handle_get_data(reinterpret_cast<uv_handle_t *>(req));
    auto _send = static_cast<libuv_stream *>(data);
    if(status != 0)
        cout<<"call write to char send fail!!"<<endl;
    _send->_send_call();
}

int libuv_stream::send_data(const string &buf,int nbuf, function<void(void)> send_call)
{
    uv_handle_set_data(reinterpret_cast<uv_handle_t*>(&(this->_wreq)),this);
    this->_send_call = send_call;
    uv_buf_t _tbuf = uv_buf_init(const_cast<char *>(buf.c_str()),buf.length());
    return uv_write(&(this->_wreq),&(this->_tstream),&_tbuf,nbuf,send_cb);
}

int libuv_stream::stoprecv_data()
{
    return uv_read_stop(&(this->_tstream));
}

}

#endif