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

    template<class ExecutorImpl>
    libuv_tcp(const executor<ExecutorImpl> &exec):
    _tloop(const_cast<uv_loop_t *>(&(static_cast<const ExecutorImpl*>(&exec) -> loop)))
    {
        uv_tcp_init(&_tloop,&_tserver);
    };
    int tcp_bind(const string & ipaddr,ip_type type, int port);

    int tcp_connect(function<void(void)> tcp_call);

    int listen_stream(backlog num,function<void(void)> s_listen_call);

    int accept_data();

    void tcp_shutdown(function<void(void)> shutdown_cb);

public:
    function<void(void)> connect_callback;
    uv_tcp_t _tserver;
    uv_tcp_t _tclient;
    uv_connect_t _connect;
public:
    function<void(void)> _listen_callback;
    function<void(void)> _shutdown_callback;
    uv_stream_t stream_server;
    uv_stream_t stream_client;
private:
    uv_loop_t _tloop;
    sockaddr_in ipv4_addr;
    sockaddr_in6 ipv6_addr;

};    

int libuv_tcp::tcp_bind(const string &ipaddr,ip_type type, int port)
{
    int ret;
    if(type == tcp<libuv_tcp>::ip_type::IPV4){
        ret = uv_ip4_addr(ipaddr.c_str(),port,&ipv4_addr);
        if(ret==0){
            return uv_tcp_bind(&_tclient,reinterpret_cast<sockaddr*>(&ipv4_addr),0);
        }
    }
    else if(type == tcp<libuv_tcp>::ip_type::IPV6){
        ret = uv_ip6_addr(ipaddr.c_str(),port,&ipv6_addr);
        if(ret==0){
            return uv_tcp_bind(&_tclient,reinterpret_cast<sockaddr*>(&ipv6_addr),0);
        }
    }
    else{
        cout<<"please input valid ip addr type!"<<endl;
    }
}

static void connect_cb(uv_tcp_t *_tcp, int status){
    auto data = uv_handle_get_data(reinterpret_cast<uv_handle_t *>(_tcp));
    auto tcp_h = static_cast<libuv_tcp *>(data);
    if(0 != status)
    {
        cout<<"tcp connect fail!!"<<endl;
    }
    tcp_h->connect_callback();
}

int libuv_tcp::tcp_connect(function<void(void)> tcp_call)
{
    uv_handle_set_data(reinterpret_cast<uv_handle_t *>(&(this->_connect)),this);
    this->connect_callback = tcp_call;
    if(tcp<libuv_tcp>::ip_type type == tcp<libuv_tcp>::ip_type::IPV4)
    {
        return uv_tcp_connect(&(this->_connect), &(this->_tclient),reinterpret_cast<sockaddr *>(&(this->ipv4_addr)),connect_cb);
    }
    else if(tcp<libuv_tcp>::ip_type type == tcp<libuv_tcp>::ip_type::IPV6)
    {
        return uv_tcp_connect(&(this->_connect), &(this->_tclient),reinterpret_cast<sockaddr *>(&(this->ipv6_addr)),connect_cb);
    }
}

static void listen_cb(uv_stream_t * server, int status)
{
    auto data = uv_handle_get_date(reinterpret_cast<uv_handle_t *>(server));
    auto stream_h = static_cast<libuv_tcp *>(data);
    if(0 != status){
        cout<<"server listen fail!!!"<<endl;
    }
}

int libuv_tcp::listen_stream(backlog num,function<void(void)> s_listen_call)
{
    uv_handle_set_data(reinterpret_cast<uv_handle_t *>(&(this->stream_server)),this);
    this->_listen_callback = s_listen_call;
    return uv_listen(&(this->stream_server),num,listen_cb);
}

int libuv_tcp::accept_data()
{
    return uv_accept(&(this->stream_server),&(this->stream_client));
}

/*static void sd_cb(uv_shutdown_t *sd,int status)
{
    auto data= uv_handle_get_data(reinterpret_cast<uv_handle_t *>(sd));
    auto shut_h = static_cast<libuv_tcp *>(data);
    if(0 != status){
        cout<<"close tcp connnect fail!!!"<<endl;
    }
}*/

void libuv_tcp::tcp_shutdown(function<void(void)> shutdown_cb)
{
    uv_shutdown(uv_showdown_t *req,this->server_stream,[](uv_shutdown_t *req,int status){
        shutdown_cb();
        if(0 != status){
            cout<<"uv_shutdown function runing fail!!!"<<endl;
        }
    })
}

}

#endif