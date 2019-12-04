#include "ual/file.h"
#include "ual/executor.h"
#include "config.h"
#include "ual/impls/impl_file.h"
#include "ual/impls/impl_executor.h"
#include <fstream>
#include<iostream>

using namespace std;
using namespace ual;

template <class Impl>
void call_c(Impl &c) {
    c.start();
    //cout<<"UV_RUN_DEFAULT seccess"<<endl;
}

void file_callback()
{
    cout<<"call file callback function "<<endl;
}

int main()
{
    libuv_executor executor;
    const string path="./test/file_txt.txt";
    libuv_file f(executor);
    f.open(path,O_RDWR ,[&f](libuv_file::errcode_t _code){
        if(_code != libuv_file::errcode_t::success) {
            return;
        } else {
            f.write("helloworld\n", -1, [&f](file<libuv_file>::errcode_t _code){
                if(_code != file<libuv_file>::errcode_t::success) {
                    cout << "success" << endl;
                    return;
                } else {
                    f.read(11, 0, [&f](libuv_file::errcode_t _code, string data){
                        cout << data << endl;
                        f.close();
                    });
                }
            });
        }
    });
    call_c(executor);
}