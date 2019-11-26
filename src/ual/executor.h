#ifndef _UAL_EXECUTOR_H
#define _UAL_EXECUTOR_H

#include "config.h"
#include "helper/this_subclass.h"

namespace UAL_NAMESPACE {

template<class Impl>
class executor: public __this_subclass<Impl> {
    using __this_subclass<Impl>::sub_this;

public:

    /*
     * default constructor function.
     *
     * create event loop or future's executor
     */
    executor() {
        sub_this -> init();
    }

    /*
     * copy constructor function is delete.
     */
    executor(const executor &exec) = delete;

    /*
     * destructor function.
     *
     * close executor and free resources.
     */
    ~executor() {
        sub_this -> destory();
    }

    /*
     * start executor.
     *
     * run executor forever or until all futures/event executed.
     */
    int start() {
        return sub_this -> run(Impl::Default);
    }

    /*
     * stop executor.
     */
    int stop() {
        return sub_this -> stop();
    }


    /*
     * run executor at once.
     *
     * execute one future/event.
     */
    int once() {
        return sub_this -> run(Impl::Once);
    }

    /*
     * poll executor.
     */
    int poll() {
        return sub_this -> run(Impl::Poll);
    }

    /*
     * now.
     *
     * got timestamp.
     */
    int64_t now() {
        return sub_this -> now();
    }
};

}

#endif

