#include "ual/udp.h"
#include "config.h"
#include "ual/impls/impl_udp.h"
#include <iostream>

using namespace std;
using namespace ual;

int main()
{
    char receive[11] = "0x00000000";
    char rep[8]="0";
    libcoap_udp ser_udp;
    ser_udp.common_bind("0.0.0.0","123456");
    ser_udp.response_session(receive,[&](const char *src,char *dst){
        cout<<"runing udp_request_context successed"<<endl;
        if(strcmp(receive,src) == 0)
        {
            strncpy(rep ,"0x111111",sizeof(rep));
            ser_udp.udp_response_data(rep);
        }
    });
    return 0;
}