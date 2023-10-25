#include <iostream>

#include "ytest.h"

using namespace std;

class Tester : public ytest::TestSuite {
public:
    static void setUpTestSuite() {
        cout << "Tester::setUpTestSuite()" << endl;
    }

    static void tearDownTestSuite() {
        cout << "Tester::tearDownTestSuite()" << endl;
    }

    virtual void setUp() override {
        cout << "Tester::setUp()" << endl;
    }

    virtual void tearDown() override {
        cout << "Tester::tearDown()" << endl;
    }
};

YTEST_F(Tester, case_name_1) {
    cout << "test Test.case_name_1 ..." << endl;
}

YTEST_F(Tester, case_name_2) {
    cout << "test Test.case_name_2 ..." << endl;
}
