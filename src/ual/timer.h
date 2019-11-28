#ifndef _UAL_TIMER
#define _UAL_TIMER

#include "config.h"
#include "ual/helper/this_subclass.h"
#include "ual/executor.h"

namespace UAL_NAMESPACE {

template <class Impl>
class timer: public __this_subclass<Impl>{

    using __this_subclass<Impl>::sub_this;

public:
    /*
     * run timer once.
     */
    bool once() {
        return repeat(1);
    }

    /*
     * run timer repeated.
     *
     * Start the timer.
     */
    bool repeat(size_t repeat) {
        _repeat = repeat;
        return sub_this -> repeat(_repeat);
    }

    /*
     * restart timer.
     *
     * Stop the timer, and if it is repeating restart it using the repeat value as the timeout.
     */
    bool again() {
        return sub_this ->repeat(_repeat);
    }

    /*
     * stop timer.
     * 
     * Stop the timer.
     */
    void stop() {
        return sub_this -> stop();
    }

    size_t _repeat;
};

}

#endif
