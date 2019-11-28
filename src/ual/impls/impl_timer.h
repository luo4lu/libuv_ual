#ifndef _UAL_IMPLS_IMPL_TIMER_H
#define _UAL_IMPLS_IMPL_TIMER_H

#include "config.h"
#include "ual/timer.h"
#include <uv.h>

namespace ual{

class libuv_timer:public timer<libuv_timer>{
public:
    using timer<libuv_timer>::timer;

    template<class ExecutorImpl>
    int init(executor<ExecutorImpl> &exec);

    int repeat(size_t repeat);

    int stop();

    void destory();

    friend timer<libuv_timer>;

public:
    size_t timeout;
    function<void(void)> handler;
    
private:
    uv_timer_t _timer;
    const uv_loop_t *t_loop;
};

}



#endif