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

class libuv_file: public file<libuv_file>{
public:
    /*
     * use executor to construct libuv_file
     */
    template<class ExecutorImpl>
    libuv_file(const executor<ExecutorImpl> &exec):
        _loop(*(static_cast<const ExecutorImpl*>(&exec) -> loop)，
        _buf(nullptr) {}

    ~libuv_file() {
        uv_fs_req_cleanup(&_file);
    }

    void on_open(const string &_path, flag_t flag, function<void(errcode_t _errcode)> _callback);
    
    void on_read(size_t length, offset_t offset, function<void(errcode_t _errcode, string _data)> _callback);
    
    void on_write(const string &data, offset_t offset, function<void(errcode_t _errcode)> _callback);
    
    int on_close();

    friend file<libuv_file>;

public:
    function<void(errcode_t _errcode)> open_callback;
    function<void(errcode_t _errcode, string _data)> read_callback;
    function<void(errcode_t _errcode)> write_callback;

private:
    uv_fs_t _file;
    uv_loop_t *_loop;
    string _result;
    uv_buf_t *_buf
};

static void on_file_open(uv_fs_t *_fs) {
    auto data = uv_handle_get_data(reinterpret_cast<uv_handle_t*>(cb));
    auto file = static_cast<libuv_file *>(data);
    if (file->result < 0) {
        this->open_callback(file::errcode_t::open_faild);
    } else {
        this->open_callback(file::errcode_t::success);
    }
}

void libuv_file::on_open(const string &_path, flag_t flag,function<void(errcode _errcode)> _callback)
{   
    uv_handle_set_data(reinterpret_cast<uv_handle_t*>(&(this->_file)),this);
    
    auto result = uv_fs_open(this->f_loop,&(this->_file),_path.c_str(),flag,xxxxx,on_file_open);
    if (result != 0) {
        _callback(file::errcode_t::open_faild);
    }
    
    this->open_callback = _callback;
}

static void on_file_read(uv_fs_t *_fs) {
    
}

void libuv_file::on_read(size_t length, offset_t offset, function<void(errcode _errcode, string _data)> _callback)
{
    uv_handle_set_data(reinterpret_cast<uv_handle_t*>(&(this->_file)),this);
    
    // qing kon
    this->_result.clear();
    this->_result.resize(length);
    
    this->_buf = uv_buf_init(this->_result.data(),length+1);

    auto result = uv_fs_read(this->f_loop,&(this->_file),this->_file.result,&_buf,1,offset,on_file_read)
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