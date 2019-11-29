#include "ual/timer.h"
#include "ual/executor.h"
#include "config.h"
#include "ual/impls/impl_timer.h"
#include "ual/impls/impl_executor.h"

#include <iostream>

using namespace std;
using namespace ual;

size_t re = 8;
template <class Impl>
void call_c(Impl &c) {
    c.start();
    cout<<"UV_RUN_DEFAULT seccess"<<endl;
}
void callback_tets()
{
    cout<<"callback function round success"<<endl;
}

int main() {
    libuv_executor li_exec;
    libuv_timer lib_t(li_exec);
    
    cout<<"timeout = "<<lib_t.timeout<<endl;
    lib_t.init(li_exec);
    int n;
    lib_t.repeat(8);
    n=lib_t._repeat;
    cout<<"timeout repeat = "<<n<<endl;

    libuv_timer lib_t2(li_exec,2000,[&lib_t2](){
        cout<<"lambda function callback"<<endl;
        lib_t2._repeat--;
        cout<<"lib_t._repeat == "<<lib_t2._repeat<<endl;
        if(lib_t2._repeat==0)
        {
            cout<<"run stop function\n";
            lib_t2.stop();
        }
    });
    cout<<"timeout = "<<lib_t2.timeout<<endl;
    lib_t2.repeat(8);
    cout<<"timeout--- repeat = "<<lib_t2._repeat<<endl;
    call_c(li_exec);
    return 0;
}

