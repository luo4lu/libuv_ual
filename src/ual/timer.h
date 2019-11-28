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
     * default constructor function is delete.
     */
    timer() = delete;

    /*
     * [temporary] copy constructor function is delete.
     */
    timer(const timer &t) = delete;

    /*
    * destructor function
    * close timer and free resources
    */
    ~timer(){
        sub_this->destory();
    }

    /*
     * use executor to construct timer.
     */
    template<class ExecutorImpl>
    timer(const executor<ExecutorImpl> &exec) {
        sub_this -> init(forward(exec));
        sub_this -> timeout = 0;
        sub_this -> handler = [](){};
    }

    /*
     * construct timer using timeout.
     * 
     * handler is empty. timeout and repeat are in milliseconds.
     */
    template<class ExecutorImpl>
    timer(const executor<ExecutorImpl> &exec, size_t timeout) {
        sub_this -> init(forward(exec));
        sub_this -> timeout = timeout;
        sub_this -> handler = [](){};
    }

    /*
     * construct timer using handler
     *
     * timeout is 0.
     */
    template<class ExecutorImpl>
    timer(const executor<ExecutorImpl> &exec, function<void(void)> handler) {
        sub_this -> init(forward(exec));
        sub_this -> timeout = 0;
        sub_this -> handler = handler;
    }

    /*
     * construct timer using timeout and handler.
     */
    template<class ExecutorImpl>
    timer(const executor<ExecutorImpl> &exec, size_t timeout,function<void(void)> handler) {
        sub_this -> init(forward(exec));
        sub_this -> timeout = timeout;
        sub_this -> handler = handler;
    }

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
