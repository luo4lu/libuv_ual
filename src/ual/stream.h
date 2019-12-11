#ifndef _UAL_STREAM_H
#define _UAL_STREAM_H

#include "config.h"
#include "ual/helper/this_subclass.h"
#include "ual/executor.h"
#include <string>

using namespace std;

namespace UAL_NAMESPACE{

template<class Impl>
class stream:public __this_subclass<Impl>{

    using __this_subclass<Impl>::sub_this;
public:
    
    void read(size_t len,string &buf,function<void(string _buf,size_t _len)> recv_call)
    {
        sub_this->read_data(len,buf,recv_call);
    }

    void send(string &buf,int nbuf, function<void(void)> send_call)
    {
        sub_this->send_data(buf, nbuf, send_call);
    }

    int stop()
    {
        sub_this->stopread_data();
    }
};


}

#endif