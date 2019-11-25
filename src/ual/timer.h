#ifndef _UAL_TIMER
#define _UAL_TIMER

#include "config.h"
#include "helper/this_subclass.h"

namespace UAL_NAMESPACE {

template <class Impl>
struct Timer: public __this_subclass<Impl>{

    using __this_subclass<Impl>::sub_this;

    Timer() {
        sub_this -> init(Impl::NotInit, [](){});
    }

    Timer(size_t timeout) {
        sub_this -> init(timeout, [](){});
    }

    Timer(function<void(void)> f) {
        sub_this -> init(Impl::NotInit, f);
    }

    Timer(size_t timeout, function<void(void)> f) {
        sub_this -> init(timeout, f);
    }

    Timer(const Timer &t) {
        sub_this -> copy(forward(t));
    }

    ~Timer() {
        sub_this -> destory();
    }

    void set_timeout(size_t timeout) {
        sub_this -> set_timeout(timeout);
    }

    void set_handler(function<void(void)> f) {
        sub_this -> set_handler(f);
    }

    bool once() {

    }

    void repeat(size_t timeout, size_t repeat) {
        sub_this -> repeat(forward<size_t>(timeout), forward<size_t>(repeat));
    }

    void again() {

    }

    void stop() {

    }
};

}

#endif
