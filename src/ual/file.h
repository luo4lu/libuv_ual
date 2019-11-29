#ifndef _UAL_FILE_SYSTEM
#define _UAL_FILE_SYSTEM

#include "config.h"
#include "ual/helper/this_subclass.h"
#include "ual/executor.h"
#include <string>

namespace UAL_NAMESPACE {

template<class Impl>
class file:public __this_subclass<Impl> {
    using __this_subclass<Impl>::sub_this;

public:
    /*
    * only read file 
    */
   int read()
   {
       return sub_this->on_read();
   }

    /*
    *only write file
    */
   int write()
   {
       return sub_this->on_write();
   }

   /*
   *read and write file
   */
    int rw_file()
    {
        return sub_this->open();
    }
   /*
   *close file and save
   */
  int close()
  {
      return sub_this->on_close();
  }

};
}
#endif