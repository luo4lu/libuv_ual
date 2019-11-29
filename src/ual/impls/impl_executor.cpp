#include "ual/impls/impl_executor.h"

namespace ual {

int libuv_executor::run(RunType t)
{
    return uv_run(&loop,static_cast<uv_run_mode> (t));
}
    
void libuv_executor::stop()
{
    uv_stop(&loop);
}

int64_t libuv_executor::now()
{
    return uv_now(&loop);
}

}

