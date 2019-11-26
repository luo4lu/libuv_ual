#ifndef _UAL_IMPLS_IMPL_EXECUTOR_H
#define _UAL_IMPLS_IMPL_EXECUTOR_H

#include "config.h"
#include "ual/executor.h"

namespace ual {

class libuv_executor: executor<libuv_executor> {
public: 
    enum class RunType {
        Default,
        Once,
        Poll,
    };

    void init();

    void destory();

    int run(RunType t);
    
    int stop();

    int64_t now();
};


}

#endif

