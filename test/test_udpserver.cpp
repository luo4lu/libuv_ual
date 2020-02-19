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
    libcoap_udp ser_udp;
    ser_udp.common_bind("0.0.0.0","12345");
    ser_udp.set_timeout(5);
    ser_udp.response_session(receive,"/test/",[&](const char *src,char *dst){
        cout<<"runing udp_request_context successed"<<endl;
        cout<<"received : "<<receive<<"\tsrc : "<<src<<endl;
        if(strncmp(receive,src,9) == 0)
        {
            strncpy(rep ,receive,sizeof(receive));
            rep[4] = 'b';
            ser_udp.udp_response_data(rep);
        }
    });
    return 0;
}