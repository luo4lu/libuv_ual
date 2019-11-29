#ifndef _UAL_IMPLS_IMPL_FILE_H
#define _UAL_IMPLS_IMPL_FILE_H

#include "config.h"
#include "ual/file.h"
#include <uv.h>
#include <string> 

using namespace std;

namespace ual{

static void file_cb(uv_fs_t *cb);
class libuv_file:public file<libuv_file>{
public:
    using file<libuv_file>::file;
    enum class OpenMode{
        R_ONLY = 00,
        W_ONLY = 01,
        R_AND_W = 02
    };
    /*
    *use executor to construct libuv_file
    */
    template<class ExecutorImpl>
    libuv_file(const executor<ExecutorImpl> &exec,string path,int flag,int mode,function<void(void)> callback){
        this->init(exec);
        this->_path = path;
        this->flags = flag;
        this->mode = mode;
        uv_fs_open(this->f_loop,&_file,this->_path,this->flags,this->mode,this->callback);
    }

    void init(const executor<ExecutorImpl> &exec);

    friend file<libuv_file>;
public:
    function<void(void)> callback;
private:
    string _path;
    int flags;
    int mode;
    uv_fs_t _file;
    uv_loop_t *f_loop;
};

static void file_cb(uv_fs_t *cb)
{
    auto data = uv_handle_get_data(reinterpret_cast<uv_handle_t*>(cb));
    auto file = static_cast<libuv_file *>(data);
    file->callback();
}

template<class ExecutorImpl>  
void libuv_file::init(const executor<ExecutorImpl> &exec)
{
    auto *impl_exec = static_cast<const ExecutorImpl*>(&exec);
    this->f_loop = const_cast<uv_loop_t*>(&impl_exec->loop);
    uv_fs_req_cleanup(&_file);
    uv_handle_set_data(reinterpret_cast<uv_handle_t*>(&(this->_file)),this);
    //return uv_fs_open(this->f_loop,&_file,this->_path,this->flags,this->mode,this->callback);
}

}
#endif