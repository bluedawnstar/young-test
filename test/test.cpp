#include <iostream>

#include "ytest.h"

using namespace std;


// lambda functions

YTEST_ADD_CASE("A", []() {
    cout << "TEST A" << endl;
    return true;
});

YTEST_ADD_CASE("AA", []() {
    cout << "TEST AA" << endl;
    return true;
});

YTEST_ADD_CASE("AB", []() {
    cout << "TEST AB" << endl;
    return true;
});


// C-style functions

bool testB() {
    cout << "TEST B" << endl;
    return true;
}

YTEST_ADD_CASE("B", testB);


// Function objects

struct TestC {
    bool operator()() const {
        cout << "TEST C" << endl;
        return true;
    }
};

YTEST_ADD_CASE("C", TestC());
