#ifndef _UAL_IMPLS_IMPL_FILE_H
#define _UAL_IMPLS_IMPL_FILE_H

#include "config.h"
#include "ual/file.h"
#include <uv.h>
#include <string> 
#include <cstring>
#include <iostream>

using namespace std;

namespace ual{

class libuv_file: public file<libuv_file>{
public:
    /*
     * use executor to construct libuv_file
     */
    template<class ExecutorImpl>
    libuv_file(const executor<ExecutorImpl> &exec):
        _loop(const_cast<uv_loop_t *>(&(static_cast<const ExecutorImpl*>(&exec) -> loop)))
         {};

    ~libuv_file() {
        if(open_req.fs_type == UV_FS_OPEN)
            uv_fs_req_cleanup(&open_req);

        if(read_req.fs_type == UV_FS_READ)
            uv_fs_req_cleanup(&read_req);

        if(write_req.fs_type == UV_FS_WRITE)
            uv_fs_req_cleanup(&write_req);
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
    string _result;
    uv_buf_t _buf;
    uv_fs_t open_req;
    uv_fs_t read_req;
    uv_fs_t write_req;
private:
    //uv_fs_t _file;
    uv_loop_t *_loop;
};

static void on_file_open(uv_fs_t *_fs) {
    auto data = uv_handle_get_data(reinterpret_cast<uv_handle_t*>(_fs));
    auto _file = static_cast<libuv_file *>(data);
    if (_file->open_req.result < 0) {
        _file->open_callback(file<libuv_file>::errcode_t::open_faild);
    } else {
        _file->open_callback(file<libuv_file>::errcode_t::success);
    }
}

void libuv_file::on_open(const string &_path, flag_t flag,function<void(errcode_t _errcode)> _callback)
{   
    uv_handle_set_data(reinterpret_cast<uv_handle_t*>(&(this->open_req)),this);
    
    auto result = uv_fs_open(this->_loop,&(this->open_req),_path.c_str(),(int)flag,0,on_file_open);
    if (result < 0) {
        _callback(file<libuv_file>::errcode_t::open_faild);
    }
    
    this->open_callback = _callback;
}

static void on_file_read(uv_fs_t *_fs) {
    auto data = uv_handle_get_data(reinterpret_cast<uv_handle_t*>(_fs));
    auto _file = static_cast<libuv_file *>(data);
    if (_file->open_req.result < 0) {
        _file->read_callback(file<libuv_file>::errcode_t::read_faild,_file->_result);
    } else {
        _file->read_callback(file<libuv_file>::errcode_t::success,_file->_result);
    }
}

void libuv_file::on_read(size_t length, offset_t offset, function<void(errcode_t _errcode, string _data)> _callback)
{
    uv_handle_set_data(reinterpret_cast<uv_handle_t*>(&(this->read_req)),this);
    
    // qing kon
    this->_result.clear();
    this->_result.resize(length);
    this->_buf = uv_buf_init(const_cast<char *>(this->_result.data()),length+1);

    auto result = uv_fs_read(this->_loop,&(this->read_req),this->open_req.result,&_buf,1,offset,on_file_read);
    if(result < 0)
    {
        _callback(file<libuv_file>::errcode_t::read_faild,const_cast<char *>(this->_result.data()));
        on_close();
    }
    this->read_callback = _callback;
}

static void on_file_write(uv_fs_t *_fs){
    auto data = uv_handle_get_data(reinterpret_cast<uv_handle_t*>(_fs));
    auto _file = static_cast<libuv_file *>(data);
    if(_file->open_req.result < 0){
        _file->write_callback(file<libuv_file>::errcode_t::write_faild);
    }else{
        _file->write_callback(file<libuv_file>::errcode_t::success);
    }
}
void libuv_file::on_write(const string &data, offset_t offset, function<void(errcode_t _errcode)> _callback)
{
    uv_handle_set_data(reinterpret_cast<uv_handle_t*>(&(this->write_req)),this);
    this->_buf = uv_buf_init(const_cast<char *>(data.c_str()),data.length());
    auto result = uv_fs_write(this->_loop,&(this->write_req),this->open_req.result,&(this->_buf),1,offset,on_file_write);
    if(result < 0)
    {
        _callback(file<libuv_file>::errcode_t::write_faild);
    }
    this->write_callback = _callback;
}
int libuv_file::on_close()
{
    uv_fs_t close_req;
    return uv_fs_close(this->_loop,&close_req,this->open_req.result,NULL);
}

}
#endif