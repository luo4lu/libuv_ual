#ifndef _UAL_IMPLS_IMPL_TCP_H
#define _UAL_IMPLS_IMPL_TCP_H

#include "config.h"
#include "ual/tcp.h"
#include <uv.h>
#include <iostream>

namespace ual{

class libuv_tcp:public tcp<libuv_tcp>{
public:
    typedef uint16_t port;

    union addr{
        uint8_t addr4[4];
        uint8_t addr6[16];
    };

    template<class ExecutorImpl,class StreamImpl>
    libuv_tcp(const executor<ExecutorImpl> &exec,const stream<StreamImpl> &strm):
    _tloop(const_cast<uv_loop_t *>(&(static_cast<const ExecutorImpl*>(&exec) -> loop)))
    {
        _ttcp = reinterpret_cast<uv_tcp_t *>(const_cast<uv_stream_t *>(&(static_cast<const StreamImpl*>(&strm) -> _tstream)));
        uv_tcp_init(_tloop,_ttcp);
    };
    int tcp_bind(const string & ipaddr,ip_type type, int port);

    int tcp_connect(ip_type type, function<void(void)> tcp_call);

    int listen_stream(backlog num,function<void(void)> s_listen_call);

    template<class StreamImpl>
    int accept_data(const stream<StreamImpl> &strm);

    void tcp_shutdown(function<void(void)> shutdown_cb);

public:
    function<void(void)> connect_callback;
    uv_tcp_t *_ttcp;
    uv_tcp_t *acc_client;
    uv_connect_t _connect;
public:
    function<void(void)> _listen_callback;
    function<void(void)> _shutdown_callback;
    uv_shutdown_t shutdown_req;

private:
    uv_loop_t *_tloop;
    sockaddr_in ipv4_addr;
    sockaddr_in6 ipv6_addr;

};    

int libuv_tcp::tcp_bind(const string &ipaddr,ip_type type, int port)
{
    int ret;
    if(type == tcp<libuv_tcp>::ip_type::IPV4){
        ret = uv_ip4_addr(ipaddr.c_str(),port,&ipv4_addr);
        if(ret==0){
            return uv_tcp_bind((this->_ttcp),reinterpret_cast<sockaddr*>(&ipv4_addr),0);
        }
    }
    else if(type == tcp<libuv_tcp>::ip_type::IPV6){
        ret = uv_ip6_addr(ipaddr.c_str(),port,&ipv6_addr);
        if(ret==0){
            return uv_tcp_bind((this->_ttcp),reinterpret_cast<sockaddr*>(&ipv6_addr),0);
        }
    }
    else{
        cout<<"please input valid ip addr type!"<<endl;
    }
}

static void connect_cb(uv_connect_t *_tcp, int status){
    auto data = uv_handle_get_data(reinterpret_cast<uv_handle_t *>(_tcp));
    auto tcp_h = static_cast<libuv_tcp *>(data);
    if(0 != status)
    {
        cout<<"tcp connect fail!!"<<endl;
    }
    tcp_h->connect_callback();
}

int libuv_tcp::tcp_connect(ip_type type, function<void(void)> tcp_call)
{
    uv_handle_set_data(reinterpret_cast<uv_handle_t *>(&(this->_connect)),this);
    this->connect_callback = tcp_call;
    if(type == tcp<libuv_tcp>::ip_type::IPV4)
    {
        return uv_tcp_connect(&(this->_connect), (this->_ttcp),reinterpret_cast<sockaddr *>(&(this->ipv4_addr)),connect_cb);
    }
    else if(type == tcp<libuv_tcp>::ip_type::IPV6)
    {
        return uv_tcp_connect(&(this->_connect), (this->_ttcp),reinterpret_cast<sockaddr *>(&(this->ipv6_addr)),connect_cb);
    }
}

static void listen_cb(uv_stream_t * server, int status)
{
    auto data = uv_handle_get_data(reinterpret_cast<uv_handle_t *>(server));
    auto stream_h = static_cast<libuv_tcp *>(data);
    if(0 != status){
        cout<<"server listen fail!!!"<<endl;
        return;
    }
    stream_h->_listen_callback();
}

int libuv_tcp::listen_stream(backlog num,function<void(void)> s_listen_call)
{
    uv_handle_set_data(reinterpret_cast<uv_handle_t *>((this->_ttcp)),this);
    this->_listen_callback = s_listen_call;
    return uv_listen((uv_stream_t *)((this->_ttcp)),num,listen_cb);
}

template<class StreamImpl>
int libuv_tcp::accept_data(const stream<StreamImpl> &strm)
{
    uv_tcp_t *_client;
    _client = (uv_tcp_t *)malloc(sizeof(uv_tcp_t));
    _client = reinterpret_cast<uv_tcp_t *>(const_cast<uv_stream_t *>(&(static_cast<const StreamImpl*>(&strm) -> _tstream)));
    uv_tcp_init(this->_tloop, _client);
    cout<<"stream transport type : "<<_client->type<<endl;
    int value = uv_accept((uv_stream_t *)((this->_ttcp)),(uv_stream_t *)_client);
    if(value != 0)
        uv_close((uv_handle_t *)_client,NULL);
    return value;
}

static void sd_cb(uv_shutdown_t *sd,int status)
{
    auto data= uv_handle_get_data(reinterpret_cast<uv_handle_t *>(sd));
    auto shut_h = static_cast<libuv_tcp *>(data);
    if(0 != status){
        cout<<"close tcp connnect fail!!!"<<endl;
    }
}

void libuv_tcp::tcp_shutdown(function<void(void)> shutdown_cb)
{
    //uv_handle_set_data(reinterpret_cast<uv_handle_t *>(&(this->shutdown_req)),this);
    
    uv_close((uv_handle_t *)this->_ttcp,NULL);
}

}

#endif