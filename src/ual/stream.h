#ifndef _UAL_STREAM_H
#define _UAL_STREAM_H

#include "config.h"
#include "ual/helper/this_subclass.h"
#include <string>

using namespace std;

namespace UAL_NAMESPACE{

template<class Impl>
class stream:public __this_subclass<Impl>{

    using __this_subclass<Impl>::sub_this;
public:
    
    int recv( function<void(const string &_buf,size_t _len)> recv_call)
    {
        return sub_this->recv_data(recv_call);
    }

    int send(const string &buf,int nbuf, function<void(void)> send_call)
    {
        return sub_this->send_data(buf, nbuf, send_call);
    }

    int stop()
    {
        sub_this->stoprecv_data();
    }
};


}

#endif