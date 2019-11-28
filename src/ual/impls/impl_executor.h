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

    libuv_executor() {
        uv_loop_init(&loop);
    }

    libuv_executor(const libuv_executor&) = delete;

    ~libuv_executor() {
        uv_loop_close(&loop);
    }

    int run(RunType t);
    
    void stop();

    int64_t now();
friend class libuv_timer;

private:
    uv_loop_t loop;
  
};


}

#endif

