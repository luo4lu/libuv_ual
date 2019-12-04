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
    typedef int offset_t;
    
    enum class flag_t {
        RD,
        WR,
        RDWD,
    };
    
    enum class errcode_t {
        success = 0,
        open_faild = 101,
        write_faild,
        read_faild
    };
    /*
     * @brief open file start.
     *
     * xxxx
     * 
     * @params 
     * @return
     * 
     */
    void open(const string &_path, int flag, function<void(errcode_t _errcode)> callback)
    {
        sub_this->on_open(_path,flag,callback);
    }
    
    /*
     * @brief read file.
     */
    void read(size_t length, offset_t offset, function<void(errcode_t _errcode, string _data)> callback)
    {
        sub_this->on_read(length,offset,callback);
    }

    /*
     * write file
     */
    void write(const string &data, offset_t offset, function<void(errcode_t _errcode)> callback)
    {
        sub_this->on_write(data,offset,callback);
    }

    /*
     * @brief close file and save
     */
    int close()
    {
        return sub_this->on_close();
    }

   // char *_pbuf;
};
}
#endif
