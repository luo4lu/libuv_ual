#include "config.h"

namespace UAL_NAMESPACE {

template <class T>
struct Timer {
    Timer() {
        T *p = static_cast<T*>(this);
        p->init();
    }

    Timer(const Timer &t) = delete;

    ~Timer() {
        T *p = static_cast<T*>(this);
        p->destory();
    }

    void repeat(size_t timeout, size_t repeat) {
        T *p = static_cast<T*>(this);
        p->repeat(forward<size_t>(timeout), forward<size_t>(repeat));
    }
};

}
