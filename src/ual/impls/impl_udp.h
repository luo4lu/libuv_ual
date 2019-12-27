#ifndef _UAL_IMPLS_IMPL_UDP_H
#define _UAL_IMPLS_IMPL_UDP_H

#include "config.h"
#include "ual/udp.h"
#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <coap2/coap.h>

namespace ual{

class libcoap_udp:public udp<libcoap_udp> {
public:
   
    libcoap_udp():_ctx(NULL),_pdu(NULL),_resource(NULL),_endpoint(NULL),_session(NULL),_ruri(NULL)
    {
        coap_startup();
    }
    int udp_common_bind(const string &dst_ipaddr,const string dst_port);

    int udp_response_session(const string & src_ipaddr, const string &src_port,function<void(string &flag)> session_call);

    int udp_request_context(const string & src_ipaddr,const string &src_port,function<void(const string &dst,string &src)>context_call);
  
    int udp_request_data(string &data);
   
    int udp_response_data(string &ack);
   
    void udp_close(); 

public:
    coap_context_t *_ctx;
    coap_pdu_t *_pdu;
    coap_address_t _dst;
    coap_resource_t *_resource;
    coap_endpoint_t *_endpoint;
    coap_session_t *_session;
    coap_str_const_t *_ruri;
public:
    function<void(string &flag)> _session_call;
    function<void(const string &dst,string &src)>_context_call;
private:
    
};

int libcoap_udp::udp_common_bind(const string &ipaddr,const string port)
{
    struct addrinfo *res,*ainfo;
    struct addrinfo hints;
    int error,len = -1;

    memset(&hints,0,sizeof(hints));
    memset(&(this->_dst),0,sizeof(this->_dst));   
    hints.ai_socktype = SOCK_DGRAM;  
    hints.ai_family = AF_UNSPEC;

    error = getaddrinfo(ipaddr.c_str(),port.c_str(),&hints,&res);
    if(error != 0)
    {
        cout<<"getaddrinfo: "<<gai_strerror(error)<<endl;
    }

    for(ainfo=res;ainfo != NULL;ainfo = ainfo->ai_next){
        switch (ainfo->ai_family)
        {
        case AF_INET6:
        case AF_INET:
            len = this->_dst.size = ainfo->ai_addrlen;
            memcpy(&(this->_dst.addr),ainfo->ai_addr,this->_dst.size);
            goto finish;
        
        default:
            break;
        }
    }
finish:
    freeaddrinfo(res);
    return len;
}

static void response_handle(struct coap_context_t *context,
                                   coap_session_t *session,
                                   coap_pdu_t *sent,
                                   coap_pdu_t *received,
                             const coap_tid_t id)
{
    coap_opt_t *block_opt;
    coap_opt_iterator_t opt_iter;
    coap_show_pdu(LOG_INFO,received);
    if((received->token_length != sent->token_length) ||
        (memcpy(received->token,sent->token,sent->token_length)!= 0)){
        cout<<"not send message struct"<<endl;
        return ;
    }
    if(!((received->type ==COAP_MESSAGE_CON) || (received->type == COAP_MESSAGE_NON))){
        cout<<"message type error"<<endl;
        return ;
    }
}

int libcoap_udp::udp_response_session(const string & ipaddr, const string &port,function<void(string &flag)> session_call)
{
    this->_ctx = coap_new_context(NULL);
    this->_session_call = session_call;
    if(!this->_ctx){
        cout<<"creat Coap context failed!!"<<endl;
        udp_close();
        return -1;
    }
    if(ipaddr.length())
    {
        int s;
        struct addrinfo hints;
        struct addrinfo *result = NULL,*rp;
        memset(&hints,0,sizeof(addrinfo));
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_DGRAM;
        hints.ai_flags = AI_PASSIVE | AI_NUMERICHOST | AI_NUMERICSERV | AI_ALL;

        s= getaddrinfo(ipaddr.c_str(),port.c_str(),&hints,&result);
        if(s != 0){
            cout<<"error getaddrinfo : "<<gai_strerror(s)<<endl;
            udp_close();
            return -1;
        }
        for(rp=result;rp != NULL;rp->ai_next){
            coap_address_t bind_addr;
            if(rp->ai_addrlen <= sizeof(bind_addr.size)){
                coap_address_init(&bind_addr);
                bind_addr.size = rp->ai_addrlen;
                memcpy(&bind_addr.addr,rp->ai_addr,rp->ai_addrlen);
                this->_session = coap_new_client_session(this->_ctx,&bind_addr,&(this->_dst),COAP_PROTO_UDP);
                if(this->_session)
                    break;
            }
        }
        freeaddrinfo(result);
    }

    coap_register_response_handler(this->_ctx,response_handle);
    this->_pdu = coap_pdu_init(COAP_MESSAGE_CON,COAP_REQUEST_GET,coap_new_message_id(this->_session),coap_session_max_pdu_size(this->_session));
    if(!this->_pdu)
    {
        cout<<"cannot create PDU\n";
        udp_close();
        return -1;
    }
    coap_add_option(this->_pdu,COAP_OPTION_URI_PATH,5,reinterpret_cast<const uint8_t *>("hello"));
    coap_send(this->_session,this->_pdu);
    coap_run_once(this->_ctx,0);
    return 0;  
}

int libcoap_udp::udp_response_data(string &ack)
{
    coap_tid_t tid;
    coap_add_option(this->_pdu,COAP_OPTION_URI_PATH,ack.length(),reinterpret_cast<const uint8_t *>(ack.c_str()));
    tid = coap_send(this->_session,this->_pdu);
    if(tid == COAP_INVALID_TID){
        cout<<"message_handler: error sending new request"<<endl;
        return -1;
    }
    coap_run_once(this->_ctx,0);
    udp_close();
    return 0;
}

static void request_handler(coap_context_t *ctx,
              struct coap_resource_t *resource,
              coap_session_t *session,
              coap_pdu_t *request,
              coap_binary_t *token,
              coap_string_t *query,
              coap_pdu_t *response)
{
    response->code = COAP_RESPONSE_CODE(205);
    coap_add_data(response,5,reinterpret_cast<const uint8_t *>("world"));
}

int libcoap_udp::udp_request_context(const string & src_ipaddr,const string &src_port,
                    function<void(const string &dst,string &src)>context_call)
{
    coap_str_const_t *ruri = coap_make_str_const("hello");
    this->_ctx = coap_new_context(nullptr);
    if(!this->_ctx || !(this->_endpoint = coap_new_endpoint(this->_ctx,&this->_dst,COAP_PROTO_UDP))){
        cout<<"connot initialize context "<<endl;
        udp_close();
        return -1;
    }
    this->_resource = coap_resource_init(ruri,0);
    coap_register_handler(this->_resource,COAP_REQUEST_GET,request_handler);
    coap_add_resource(this->_ctx,this->_resource);
    while(true)
    {
        coap_run_once(this->_ctx,0);
    }
    udp_close();
    return 0;
}
  
int libcoap_udp::udp_request_data(string &data)
{

}
   

void libcoap_udp::udp_close()
{
    if(this->_session != NULL)
        coap_session_release(this->_session);
    coap_free_context(this->_ctx);
    coap_cleanup();
}

}


#endif