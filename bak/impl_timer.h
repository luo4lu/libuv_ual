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
    int init(const executor<ExecutorImpl> &exec);

    int repeat(size_t repeat);

    int stop();

    void destory();

    friend timer<libuv_timer>;

public:
    size_t timeout;
    function<void(void)> handler;
    
private:
    uv_timer_t _timer;
    uv_loop_t *t_loop;
};

static void timer_cb (uv_timer_t *handle) {
    auto data = uv_handle_get_data(reinterpret_cast<uv_handle_t*>(handle));
    auto timer = static_cast<libuv_timer *>(data);
    timer->handler();
}

template<class ExecutorImpl>
int libuv_timer::init(const executor<ExecutorImpl> &exec)
{
    auto *impl_exec = static_cast<const ExecutorImpl*>(&exec);
    this->t_loop = const_cast<uv_loop_t*>(&impl_exec->loop);
    uv_handle_set_data(reinterpret_cast<uv_handle_t*>(&(this->_timer)), this);
    return uv_timer_init(this->t_loop,&(this->_timer));
}

int libuv_timer::repeat(size_t _repeat)
{
    return uv_timer_start(&(this->_timer),timer_cb,timeout,_repeat);
}

int libuv_timer::stop()
{
    return uv_timer_stop(&(this->_timer));
}

void libuv_timer::destory()
{
}

}



#endif
