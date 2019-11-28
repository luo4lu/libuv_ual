#include "ual/impls/impl_timer.h"


namespace ual{

static void timer_cb (uv_timer_t *handle) {
    auto data = uv_handle_get_data(reinterpret_cast<uv_handle_t*>(handle));
    auto timer = static_cast<libuv_timer *>(data);
    timer->handler();
}

    template<class ExecutorImpl>
    int libuv_timer::init(executor<ExecutorImpl> &exec)
    {
        auto *loop = &(exec.loop);
        this->t_loop = static_cast<ExecutorImpl>(loop);
        uv_handle_set_data(const_cast<uv_handle_t*>(&(this->_timer)), this);
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