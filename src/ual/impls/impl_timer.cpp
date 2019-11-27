#include "ual/impls/impl_timer.h"

namespace ual{

    template<class ExecutorImpl>
    int libuv_timer::init(executor<ExecutorImpl> &exec)
    {
        t_loop = static_cast<ExecutorImpl>(&(exec.loop));
        return uv_timer_init(t_loop,&time);
    }

    uint64_t libuv_timer::repeat(_repeat)
    {
        return uv_timer_start(&time,handle,timeout,_repeat);;
    }

    int libuv_timer::stop()
    {
        return uv_timer_stop(&time);
    }

    void libuv_timer::destory()
    {
        if(uv_is_active(&time))
            uv_timer_stop(&time);

        uv_loop_close(t_loop);
    }

}