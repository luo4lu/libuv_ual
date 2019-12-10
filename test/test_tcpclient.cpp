#include "ual/tcp.h"
#include "ual/executor.h"
#include "config.h"
#include "ual/impls/impl_tcp.h"
#include "ual/impls/impl_executor.h"
#include <iostream>

using namespace std;
using namespace ual;

template <class Impl>
void call_c(Impl &c) {
    c.start();
    //cout<<"UV_RUN_DEFAULT seccess"<<endl;
}

int main()
{
    libuv_executor executor;
    const string &ip = "0.0.0.0";
    const int port = 1280;
    libuv_tcp tcp_client(executor,libuv_tcp::point_mode::CLIENT);

    tcp_client.bind(ip,libuv_tcp::ip_type::IPV4,port);
    tcp_client.connect(libuv_tcp::ip_type::IPV4,[&tcp_client](){
        cout<<"connect callback success,do not data send or recv "<<endl;
        for(int i=0;i<1000;i++)
            for(int j=0;i<1000;j++)
                ;
        tcp_client.shutdown([](){
            cout<<"close tcp transport server!!"<<endl;
        });

    });
    call_c(executor);
}