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
    const string path="./test/file_txt.txt";
    /*ifstream fopen("./test/file_txt.txt");
    if(!fopen.is_open())
    {
        cout<<"file open failure";
        return -1;
    }*/
}