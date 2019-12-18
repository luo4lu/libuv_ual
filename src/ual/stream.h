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
    
    /*
     *@brief recv or read in the stream massage
     *
     * @params recv_call:recv massage buffer and length through recv_call callback function realize
     * 
     * @return succeed: 0     failed: other 
     */
    int recv( function<void(const string &_buf,size_t _len)> recv_call)
    {
        return sub_this->recv_data(recv_call);
    }

    /*
     *@brief send or write to stream massage
     *
     * @params buf:want to send save massage the buffer
     * @params nbuf:need group number nbuf send buffer.
     * @params send_call:send massage callback function realize
     * 
     * @return succeed: 0     failed: other 
     */
    int send(const string &buf,int nbuf, function<void(void)> send_call)
    {
        return sub_this->send_data(buf, nbuf, send_call);
    }

    /*
     *@brief stop recv massage in the stream
     *
     * @params 
     * 
     * @return succeed: 0     failed: other 
     */
    int stop()
    {
        sub_this->stoprecv_data();
    }
};


}

#endif