#include "ual/timer.h"

#include <iostream>

using namespace std;

template<class Impl>
struct Test {
    Test() {
        Impl *p = static_cast<Impl*>(this);
        cout<< "call Test constructor" << endl;
        p -> init();
        p -> i = 1;
    }

    Test(const Test &t) = delete;

    ~Test() {
        Impl *p = static_cast<Impl*>(this);
        cout<< "call Test destory" << endl;
        p -> destory();
    }
};

struct TestImpl: Test<TestImpl> {
    using Test::Test;

    void init() {
        cout << "call init" << endl;
    }

    void destory() {
        cout << "call destory" << endl;
    }
    int i;
};


int main() {
    Test<TestImpl> t;
    return 0;
}

