#ifndef _UAL_FILE_SYSTEM
#define _UAL_FILE_SYSTEM

#include "config.h"
#include "ual/helper/this_subclass.h"
#include "ual/executor.h"
#include <string>

using namespace std;
namespace UAL_NAMESPACE {

template<class Impl>
class file:public __this_subclass<Impl> {
    using __this_subclass<Impl>::sub_this;

public:
    typedef int flag_t;
    typedef int offset_t;
    typedef int mode_t;
    
    enum class errcode {
        success = 0,
        open_faild = 101,
        write_faild,
        read_faild
    };
    /*
    * open file start 
    */
    void open(const string &_path, flag_t flag, mode_t mode, function<void(errcode _errcode)> callback)
    {
        sub_this->on_open(_path,flag,callback);
    }
    /*
    * only read file 
    */
    void read(size_t length, offset_t offset, function<void(errcode _errcode, string _data)> callback)
    {
        sub_this->on_read(length,offset,callback);
    }

    /*
    *only write file
    */
    void write(const string &data, offset_t offset, function<void(errcode _errcode)> callback)
    {
        sub_this->on_write(data,offset,callback);
    }

   /*
   *init need buffer for read or write
   */
    /*void init(char *pbuf)
    {
        _pbuf=pbuf;
        sub_this->buf_init(_pbuf);
    }*/
   /*
   *close file and save
   */
    int close()
    {
        return sub_this->on_close();
    }

   // char *_pbuf;
};
}
#endif