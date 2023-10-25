#include <iostream>

#include "ytest.h"

using namespace std;

class Tester2 : public ytest::TestSuite {
public:
    static void setUpTestSuite() {
        cout << "Tester2::setUpTestSuite()" << endl;
    }

    static void tearDownTestSuite() {
        cout << "Tester2::tearDownTestSuite()" << endl;
    }

    virtual void setUp() override {
        cout << "Tester2::setUp()" << endl;
    }

    virtual void tearDown() override {
        cout << "Tester2::tearDown()" << endl;
    }
};

YTEST_F(Tester2, case_name_1) {
    cout << "test Test2.case_name_1 ..." << endl;
}

YTEST_F(Tester2, case_name_2) {
    cout << "test Test2.case_name_2 ..." << endl;
    YEXPECT_TRUE(!3 != 0);
    YEXPECT_FALSE(!3 == 0);
    YEXPECT_EQ(!3, true);
    YEXPECT_NE(!3, false);
}
