#include <thread>
#include <iostream>

#include "ytest.h"

using namespace std;


// lambda functions

YTEST_CASE("A", [](const ytest::TestContext&) {
    return true;
});

YTEST_CASE("AA", [](const ytest::TestContext&) {
    this_thread::sleep_for(10ms);
    return true;
});

YTEST_CASE("AB", [](const ytest::TestContext&) {
    this_thread::sleep_for(100ms);
    return false;
});


// C-style functions

bool testB(const ytest::TestContext&) {
    return true;
}

YTEST_CASE("B", testB);


// Function objects

struct TestC {
    bool operator()(const ytest::TestContext&) const {
        return true;
    }
};

YTEST_CASE("C", TestC());
