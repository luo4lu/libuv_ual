#include "ual/udp.h"
#include "config.h"
#include "ual/impls/impl_udp.h"
#include <iostream>

using namespace std;
using namespace ual;

int main()
{
    libcoap_udp client_udp;
    char data[1024];
    client_udp.common_bind("0.0.0.0","123456");
    client_udp.request_context("127.0.0.1","12345","hello",[&](string flag){
        cout<<"runing test udp client\n"<<"data:"<<flag<<endl;
        client_udp.get_data(flag);
    });
    return 0;
}