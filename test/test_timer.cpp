#include "ual/timer.h"

#include <iostream>

using namespace std;

template<class Impl>
struct Test {

    Test() {
        Impl *p = static_cast<Impl*>(this);
        cout<< "call Test constructor" << endl;
        p -> init();
    }

    Test(const Test &t) = delete;

    ~Test() {
        Impl *p = static_cast<Impl*>(this);
        cout<< "call Test destory" << endl;
        p -> destory();
    }

    int get_i() {
        return 1;
    }

};

struct TestImpl: public Test<TestImpl> {
    friend Test<TestImpl>;

    void init() {
        cout << "call init" << endl;
        this -> i = 1;
    }

    void destory() {
        cout << "call destory" << endl;
    }

    using Test<TestImpl>::Test;

    int i;
};

template <class Impl>
void call_t(Test<Impl> &t) {
    cout << t.get_i() << endl;
}

struct TimerTest: public ual::timer<TimerTest> {

};

int main() {
    TestImpl t;
    call_t(t);
    return 0;
}

