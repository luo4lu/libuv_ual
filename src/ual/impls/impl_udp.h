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
#include "uv.h"

//using namespace libcoap;
namespace ual{
char * tmp=NULL;
coap_address_t *pp_addr = NULL;//用于回调函数指向类成员
class libcoap_udp:public udp<libcoap_udp> {
public:
   
    libcoap_udp():_ctx(NULL),_pdu(NULL),_resource(NULL),_endpoint(NULL),_session(NULL),_ruri(NULL),
    _response_data(NULL),_second(0)
    {
        coap_startup();
    }
    int udp_common_bind(const string &dst_ipaddr,const string &dst_port);

    int udp_request_context(const string & src_ipaddr, const string &src_port,const char *data,function<void(char * flag)> session_call);

    int udp_response_session(const char *key,const char *value,function<void(const char *src,char *dst)>context_call);
  
    int udp_get_data(char *data);
   
    int udp_response_data(char *ack);

    int udp_set_timeout(unsigned int sec);
   
    void udp_close(); 

public:
    coap_context_t *_ctx;
    coap_pdu_t *_pdu;
    coap_address_t _dst;
    coap_resource_t *_resource;
    coap_endpoint_t *_endpoint;
    coap_session_t *_session;
    coap_str_const_t *_ruri;
    unsigned int _second;
public:
    function<void(char * flag)> _session_call;
    function<void(const char *src,char *dst)>_context_call;
    char *_response_data;
private:
    
};

int libcoap_udp::udp_common_bind(const string &ipaddr,const string &port)
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
            memcpy(&(this->_dst.addr.sin6),ainfo->ai_addr,this->_dst.size);
            goto finish;
        
        default:
            break;
        }
    }
finish:
    freeaddrinfo(res);
    return len;
}

static void request_handle(struct coap_context_t *context,
                                   coap_session_t *session,
                                   coap_pdu_t *sent,
                                   coap_pdu_t *received,
                                   coap_tid_t id)
{
    libcoap_udp *p_pdu = CONTAINING_RECORD(pp_addr, libcoap_udp, _dst);
    coap_show_pdu(LOG_INFO,received); 
    p_pdu->_session_call((char *)received->data);
}

int libcoap_udp::udp_request_context(const string & ipaddr, const string &port,const char *data, 
                                        function<void(char *flag)> session_call)
{
    coap_tid_t tid;
    coap_startup();
    this->_ctx = coap_new_context(NULL);
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
            if(rp->ai_addrlen <= sizeof(bind_addr.addr)){
                coap_address_init(&bind_addr);
                bind_addr.size = rp->ai_addrlen;
                memcpy(&bind_addr.addr,rp->ai_addr,rp->ai_addrlen);
                this->_session = coap_new_client_session(this->_ctx,&(this->_dst),&bind_addr,COAP_PROTO_UDP);
                if(this->_session)
                    break;
            }
        }
        freeaddrinfo(result);
    }
    coap_register_response_handler(this->_ctx,request_handle);
    this->_pdu = coap_pdu_init(COAP_MESSAGE_CON,COAP_REQUEST_GET,coap_new_message_id(this->_session),coap_session_max_pdu_size(this->_session));
    if(!this->_pdu)
    {
        cout<<"cannot create PDU\n";
        udp_close();
        return -1;
    }
    coap_add_option(this->_pdu,COAP_OPTION_URI_PATH,strlen(data),reinterpret_cast<const uint8_t *>(data));
    tid = coap_send(this->_session,this->_pdu);
    if(tid == COAP_INVALID_TID){
        cout<<"message_handler: error sending new request"<<endl;
        return -1;
    }
    pp_addr = &this->_dst;
    this->_session_call = session_call;
    coap_run_once(this->_ctx,this->_second);
    udp_close();
    return 0;  
}

int libcoap_udp::udp_get_data(char* data)
{
    size_t len = strlen(data);
    cout<<"received data len = "<<len<<endl;
    if(len>0)
        cout<<"received data = "<<data<<endl;
    //free(recive_data);
    return 0;
}

static void response_handler(coap_context_t *ctx,
              struct coap_resource_t *resource,
              coap_session_t *session,
              coap_pdu_t *request,
              coap_binary_t *token,
              coap_string_t *query,
              coap_pdu_t *response)
{
    libcoap_udp *p_pdu = CONTAINING_RECORD(pp_addr, libcoap_udp, _dst);
    coap_show_pdu(LOG_INFO,request);
    p_pdu->_context_call((char *)resource->uri_path->s,p_pdu->_response_data);
    response->code = COAP_RESPONSE_CODE(205);
    coap_add_data(response,strlen(p_pdu->_response_data),reinterpret_cast<const uint8_t *>(p_pdu->_response_data));
    if(p_pdu->_response_data != NULL)
    {
        free(p_pdu->_response_data);
        p_pdu->_response_data = NULL;
    }
    cout<<"receive request data: "<<resource->uri_path->s<<endl;
    cout<<"has been send response data :"<<response->data<<endl;
}

int libcoap_udp::udp_response_session(const char *key,const char *value,
                                    function<void(const char *src,char *dst)>context_call)
{   
    coap_startup();
    coap_str_const_t *ruri = coap_make_str_const(key);
    this->_ctx = coap_new_context(nullptr);
    if(!this->_ctx || !(this->_endpoint = coap_new_endpoint(this->_ctx,&this->_dst,COAP_PROTO_UDP))){
        cout<<"connot initialize context "<<endl;
        udp_close();
        return -1;
    }
    this->_resource = coap_resource_init(ruri,0);
    coap_register_handler(this->_resource,COAP_REQUEST_GET,response_handler);
    coap_add_attr(this->_resource,coap_make_str_const("title"),coap_make_str_const(value),0);
    coap_add_resource(this->_ctx,this->_resource);
    pp_addr = &this->_dst;
    this->_context_call = context_call;
    while(true)
    {   
        coap_run_once(this->_ctx,this->_second);
    }
    udp_close();
    return 0;
}

int libcoap_udp::udp_response_data(char *ack)
{
    if(this->_response_data!=NULL)
    {
        free(this->_response_data);
        this->_response_data = NULL;
    }
    int len = strlen(ack)+1;
    this->_response_data = (char*)malloc(len);
    strncpy(this->_response_data,ack,len);
    return 0;
}

int libcoap_udp::udp_set_timeout(unsigned int second)
{
    if(second>0)
    {
        this->_second = second*1000;
        return 0;
    }
    else
    {
        cout<<"set timeout should compare 0 big";
        return -1;
    }
    
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