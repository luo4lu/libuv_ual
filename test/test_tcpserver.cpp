#include "ual/tcp.h"
#include "ual/executor.h"
#include "config.h"
#include "ual/impls/impl_tcp.h"
#include "ual/impls/impl_stream.h"
#include "ual/impls/impl_executor.h"
#include <iostream>

using namespace std;
using namespace ual;

template <class Impl>
void call_c(Impl &c) {
    cout<<"UV_RUN_DEFAULT seccess"<<endl;
    c.start();
}

int main()
{
    libuv_executor executor;
    libuv_stream stream;
    string buf;
    const string &ip = "0.0.0.0";
    const int port = 1280;
    libuv_tcp tcp_server(executor,stream);
    tcp_server.bind(ip,libuv_tcp::ip_type::IPV4,port);
    cout<<"bind----------\n";
    tcp_server.listen(128,[&](){
        cout<<"listen to ipaddr :"<<ip<<" port :"<<port<<endl;
        tcp_server.accept();
        cout<<"++++++++++++"<<endl;
        stream.recv(50,buf,[&](const string &buf,size_t len){
            cout<<"recv: "<<buf<<endl;
            cout<<"recv char len : "<<len<<endl;
        });
    });
    call_c(executor);
}