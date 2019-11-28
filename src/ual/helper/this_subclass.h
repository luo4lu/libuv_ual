#ifndef _UAL_HELPER_THIS_SUBCLASS
#define _UAL_HELPER_THIS_SUBCLASS

#include "config.h"

namespace UAL_NAMESPACE {

template <class Impl>
struct __this_subclass {

    Impl *sub_this;

    __this_subclass() {
        this -> sub_this = static_cast<Impl*>(this);
    }
};

}

#endif

