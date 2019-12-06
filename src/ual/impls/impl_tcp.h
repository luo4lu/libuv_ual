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
        uv_tcp_init(&_tloop,_tserver);
    };
    int tcp_bind(const string & ipaddr,ip_type type, int port);

    int tcp_connect(function<void(void)> tcp_call);

    int tcp_listen();

    void tcp_abort(string &ipaddr)

public:
    uv_tcp_t _tserver;
    uv_tcp_t _tclient;
    uv_connect_t _connect;

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
            uv_tcp_bind(&_tserver,reinterpret_cast<sockaddr*>(&ipv4_addr),0);
        }
    }
    else if(type == tcp<libuv_tcp>::ip_type::IPV6){
        ret = uv_ip6_addr(ipaddr.c_str(),port,&ipv6_addr);
        if(ret==0){
            uv_tcp_bind(&_tserver,reinterpret_cast<sockaddr*>(&ipv6_addr),0);
        }
    }
    else{
        cout<<"please input valid ip addr type!"<<endl;
    }
}


}