#include "ual/udp.h"
#include "config.h"
#include "ual/impls/impl_udp.h"
#include <iostream>

using namespace std;
using namespace ual;

int main()
{
    string rep = "world";
    libcoap_udp ser_udp;
    ser_udp.common_bind("0.0.0.0","123456");
    ser_udp.request_context("hello",rep,[](const string &dst,string &src){
        cout<<"runing udp_request_context successed"<<endl;
    });
    return 0;
}