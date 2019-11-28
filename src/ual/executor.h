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
     * start executor.
     *
     * run executor forever or until all futures/event executed.
     */
    int start() {
        return sub_this -> run(Impl::RunType::Default);
    }

    /*
     * stop executor.
     */
    void stop() {
        return sub_this -> stop();
    }


    /*
     * run executor at once.
     *
     * execute one future/event.
     */
    int once() {
        return sub_this -> run(Impl::RunType::Once);
    }

    /*
     * poll executor.
     */
    int poll() {
        return sub_this -> run(Impl::RunType::Poll);
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

