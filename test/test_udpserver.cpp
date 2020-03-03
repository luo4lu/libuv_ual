#include "ual/udp.h"
#include "config.h"
#include "ual/impls/impl_udp.h"
#include <iostream>
#include <string.h>

using namespace std;
using namespace ual;

int main()
{
    char receive[12] = {"0x00a0x03"};
    char rep[1024]={"0"};
    char send[12]={"0x00a0x03"};
    libcoap_udp ser_udp;
    libcoap_udp ser_udp2;
    ser_udp.common_bind("127.0.0.1","12345");
    ser_udp2.common_bind("127.0.0.1","6767");
    ser_udp.set_timeout(5);
    ser_udp.response_session(receive,libcoap_udp::request_type::POST,[&](const char *src,char *dst){
        cout<<"runing udp_request_context successed"<<endl;
        cout<<"received : "<<receive<<"\tsrc : "<<src<<endl;
        if(strncmp(receive,src,9) == 0)
        {
            strncpy(rep ,receive,sizeof(receive));
            rep[4] = 'b';
            ser_udp.udp_response_data(rep);
        }
        ser_udp2.request_context("127.0.0.1","12345",send,libcoap_udp::request_type::POST,[&](char * flag){
            cout<<"runing test udp client\n"<<"data:"<<flag<<endl;
            if(ser_udp2.show_data(flag) != 0)
            {
                cout<<"get data failed!!"<<endl;
                
                return -1;
            }
        });
    });
    return 0;
}