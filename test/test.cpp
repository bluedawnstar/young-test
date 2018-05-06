#include <iostream>

#include "ytest.h"

using namespace std;

bool testB() {
    cout << "TEST B" << endl;
    return true;
}

struct TestC {
    bool operator()() const {
        cout << "TEST C" << endl;
        return true;
    }
};

YTEST_ADD_CASE("a", []() {
    cout << "TEST A" << endl;
    return true;
});

YTEST_ADD_CASE("b", testB);
YTEST_ADD_CASE("c", TestC());
