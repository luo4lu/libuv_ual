#include "ual/timer.h"
#include "ual/executor.h"
#include "config.h"
#include "ual/impls/impl_timer.h"
#include "ual/impls/impl_executor.h"

#include <iostream>

using namespace std;
using namespace ual;

template <class Impl>
void call_c(Impl &c) {
    c.once();
    cout<<"UV_RUN_ONCE seccess"<<endl;
    c.stop();
    c.now();
    cout<<"UV_RUN_NOWAIT seccess"<<endl;
    c.stop();
    c.start();
    cout<<"UV_RUN_DEFAULT seccess"<<endl;
}


int main() {
    libuv_executor li_exec;
    libuv_timer lib_t(li_exec);
    call_c(li_exec);
    return 0;
}

