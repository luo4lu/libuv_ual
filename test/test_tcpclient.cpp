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
    c.start();
    //cout<<"UV_RUN_DEFAULT seccess"<<endl;
}

int main()
{
    libuv_executor executor;
    libuv_stream stream;
    const string &buf = "hello server,It's me Client!";
    const string &ip = "0.0.0.0";
    const int port = 1280;
    libuv_tcp tcp_client(executor,stream);

    tcp_client.bind(ip,libuv_tcp::ip_type::IPV4,port);
    tcp_client.connect(libuv_tcp::ip_type::IPV4,[&tcp_client,&stream,&buf](){
        cout<<"connect callback success,do not data send or recv "<<endl;
        for(int i=0;i<1000;i++)
            for(int j=0;j<1000;j++)
                ;
        stream.send(buf,5,[&](){
            cout<<"callback stream send function!!"<<endl;
        });
        //tcp_client.shutdown([](){
            cout<<"close tcp transport server!!"<<endl;
      //  });

    });
    call_c(executor);
}