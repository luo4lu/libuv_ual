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
    char send[12]={"0x00a0x03"};
    cout<<"----------------"<<sizeof(send)<<endl;
    int i = 0;
    client_udp.set_timeout(5);
    while(i<20)
    {
        cout<<"i == "<<i++<<endl;
    client_udp.common_bind("0.0.0.0","9999");
    client_udp.request_context("127.0.0.1","12345",send,[&](char * flag){
        cout<<"runing test udp client\n"<<"data:"<<flag<<endl;
        if(client_udp.get_data(flag) != 0)
        {
            cout<<"get data failed!!"<<endl;
            
            return -1;
        }

    });
    }
    return 0;
}