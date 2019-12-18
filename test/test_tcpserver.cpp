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
    libuv_stream stream2;
    string buf2 = "form server\t";
  //  string buf;
    const string &ip = "127.0.0.1";
    const int port = 12345;
    libuv_tcp tcp_server(executor,stream);
    tcp_server.bind(ip,libuv_tcp::ip_type::IPV4,port);
    cout<<"bind----------\n";
    int l_value = tcp_server.listen(128,[&](){
        cout<<"listen to ipaddr :"<<ip<<" port :"<<port<<endl;
        int a_value = tcp_server.accept(stream2);
        cout<<"accpet return value = "<<a_value<<endl;
        int recv_return = stream2.recv([&](const string &buf,size_t len){
            cout<<"recv: "<<buf<<endl;
            cout<<"recv char len : "<<len<<" buf = "<<strlen(buf.c_str())<<endl;
            buf2 +=buf;
            stream2.send(buf2,1,[&](){
                cout<<"data send success!!"<<endl;
            });
        });
        cout<<"recv_return value = "<<recv_return<<endl;
    });
    cout<<"listen return value = "<<l_value<<endl;
    call_c(executor);
}