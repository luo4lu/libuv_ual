#ifndef _UAL_IMPLS_IMPL_TIMER_H
#define _UAL_IMPLS_IMPL_TIMER_H

#include "config.h"
#include "ual/timer.h"
#include <uv.h>

namespace ual{

class libuv_timer:public timer<libuv_timer>{
public:
    template<class ExecutorImpl>
    int init(executor<ExecutorImpl> &exec);

    uint64_t repeat(size_t _repeat);

    int stop();

    void destory();

    friend timer<libuv_timer>;
private:
    size_t timeout;
    uv_timer_t time;
    function<void(void)> handle;
    const uv_loop_t *t_loop;
}

}



#endif