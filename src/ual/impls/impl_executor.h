#ifndef _UAL_IMPLS_IMPL_EXECUTOR_H
#define _UAL_IMPLS_IMPL_EXECUTOR_H

#include "config.h"
#include "ual/executor.h"
#include <uv.h>

namespace ual {

class libuv_executor: public executor<libuv_executor> {
public: 
    enum class RunType {
        Default = 0,
        Once,
        Poll,
    };

    void init();

    void destory();

    int run(RunType t);
    
    void stop();

    int64_t now();

friend executor<libuv_executor>;

private:
    uv_loop_t loop;
  //  uv_run_mode mode;
};


}

#endif

