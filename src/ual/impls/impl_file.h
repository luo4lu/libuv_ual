#ifndef _UAL_IMPLS_IMPL_FILE_H
#define _UAL_IMPLS_IMPL_FILE_H

#include "config.h"
#include "ual/file.h"
#include <uv.h>
#include <string> 
#include <cstring>
#include <iostream>
#include <future>
#include <chrono>

using namespace std;

namespace ual{

static void file_cb(uv_fs_t *cb);
class libuv_file:public file<libuv_file>{
public:
    using file<libuv_file>::file;
    /*
    *use executor to construct libuv_file
    */
    template<class ExecutorImpl>
    libuv_file(const executor<ExecutorImpl> &exec){
        this->f_init(exec);
        this->callback = [](){};
    }
    ~libuv_file()
    {
        uv_fs_req_cleanup(&_file);
    }
    template<class ExecutorImpl>
    void f_init(const executor<ExecutorImpl> &exec);
    void on_open(const string &_path, flag_t flag, mode_t mode, function<void(errcode _errcode)> _callback);
    void on_read(size_t length, offset_t offset, function<void(errcode _errcode, string _data)> _callback);
    void on_write(const string &data, offset_t offset, function<void(errcode _errcode)> _callback);
    int on_close();

    friend file<libuv_file>;
public:
    function<void(void)> callback;
    errcode _errcode;
private:
    uv_fs_t _file;
    uv_fs_t _read_file;
    uv_fs_t _write_file;
    uv_loop_t *f_loop;
};

static void file_cb(uv_fs_t *cb)
{
    auto data = uv_handle_get_data(reinterpret_cast<uv_handle_t*>(cb));
    auto file = static_cast<libuv_file *>(data);
    file->callback();
//    file->_file =cb;
}

template<class ExecutorImpl>  
void libuv_file::f_init(const executor<ExecutorImpl> &exec)
{
    auto *impl_exec = static_cast<const ExecutorImpl*>(&exec);
    this->f_loop = const_cast<uv_loop_t*>(&impl_exec->loop);
    //uv_handle_set_data(reinterpret_cast<uv_handle_t*>(&(this->_file)),this);
}

void libuv_file::on_open(const string &_path, flag_t flag, mode_t mode,function<void(errcode _errcode)> _callback)
{   
    uv_handle_set_data(reinterpret_cast<uv_handle_t*>(&(this->_file)),this);
    if(uv_fs_open(this->f_loop,&(this->_file),_path.c_str(),flag,mode,file_cb))
    {
        this->_errcode = errcode::success;
    }
    else
    {
       this-> _errcode = errcode::open_faild;
    }
    _callback(this->_errcode);
    
}
void libuv_file::on_read(size_t length, offset_t offset, function<void(errcode _errcode, string _data)> _callback)
{
    uv_handle_set_data(reinterpret_cast<uv_handle_t*>(&(this->_file)),this);
    char rbuf[length+1]={0};
    uv_buf_t _rbuf_t = uv_buf_init(rbuf,length+1);
    if(uv_fs_read(this->f_loop,&(this->_file),this->_file.result,&_rbuf_t,1,offset,file_cb))
    {
        this->_errcode = errcode::success;
    }
    else
    {
        this-> _errcode = errcode::read_faild;
    }
    _callback(this-> _errcode,rbuf);
}
void libuv_file::on_write(const string &data, offset_t offset, function<void(errcode _errcode)> _callback)
{
    uv_handle_set_data(reinterpret_cast<uv_handle_t*>(&(this->_file)),this);
    char *c;
    strcpy(c,data.c_str());
    uv_buf_t _wbuf_t = uv_buf_init(c,sizeof(data));
    if(uv_fs_write(this->f_loop,&(this->_file),this->_file.result,&_wbuf_t,1,offset,file_cb))
    {
        this->_errcode = errcode::success;
    }
    else
    {
        this->_errcode = errcode::write_faild;
    }
    _callback(this->_errcode);
}
int libuv_file::on_close()
{
    return uv_fs_close(this->f_loop,&(this->_file),this->_file.result,file_cb);
   // uv_fs_req_cleanup(&(this->_file));
}

}
#endif