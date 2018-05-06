#include <iostream>

#include "ytest.h"

using namespace std;

bool gTestA = false;
bool gTestB = false;
bool gTestC = false;

bool testB() {
    cout << "TEST B" << endl;
    gTestB = true;
    return true;
}

struct TestC {
    bool operator()() const {
        cout << "TEST C" << endl;
        gTestC = true;
        return true;
    }
};

YTEST_ADD_CASE("a", []() {
    cout << "TEST A" << endl;
    gTestA = true;
    return true;
});

YTEST_ADD_CASE("b", testB);
YTEST_ADD_CASE("c", TestC());
