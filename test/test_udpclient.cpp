#include "ual/udp.h"
#include "config.h"
#include "ual/impls/impl_udp.h"
#include <iostream>

using namespace std;
using namespace ual;

int main()
{
    libcoap_udp client_udp;
    client_udp.common_bind("0.0.0.0","123456");
    client_udp.response_session("127.0.0.1","12345","hello",[&](const string &flag){
        cout<<"runing test udp client\n"<<"data:"<<flag<<endl;
    });
    return 0;
}