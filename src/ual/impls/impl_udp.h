#ifndef _UAL_IMPLS_IMPL_UDP_H
#define _UAL_IMPLS_IMPL_UDP_H

#include "config.h"
#include "ual/udp.h"
#include <iostream>
#include <coap2/coap.h>

namespace ual{

class libcoap_udp:public udp<libcoap_udp> {
public:
   
    libcoap_udp():_ctx(NULL),_pdu(NULL),_resource(NULL),_endpoint(NULL),_session(NULL)
    {
        coap_startup();
    }
    int udp_common_bind(const string &ipaddr,const string port);

    int udp_Client_session(const string & ipaddr, const string &port,function<void(string &flag)> session_call);

    int udp_Server_context(const string & ipaddr,const string &port,function<void(const string &dst,string &src)>context_call);
  
    int udp_request(string &data);
   
    int udp_response(string &ack);
   
    void udp_close(); 

public:
    coap_context_t *_ctx;
    coap_pdu_t *_pdu;
    coap_address_t _dst;
    coap_resource_t *_resource;
    coap_endpoint_t *_endpoint;
    coap_session_t *_session;
private:
    
};



}


#endif