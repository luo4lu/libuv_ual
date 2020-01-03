#include "ual/udp.h"
#include "config.h"
#include "ual/impls/impl_udp.h"
#include <iostream>

using namespace std;
using namespace ual;

int main()
{
    string rep("world",5);
    libcoap_udp ser_udp;
    ser_udp.common_bind("0.0.0.0","123456");
    ser_udp.response_session("hello",rep,[&](){
        cout<<"runing udp_request_context successed"<<endl;
    });
    return 0;
}