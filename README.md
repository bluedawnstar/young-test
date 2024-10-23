# young-test
Youngman's very simple test-framework

## How To Use

### Step1: add ```./include``` to 'include' paths of your projects

### Step2: add the test functions and function objects you want to call as shown below.

  - global test cases
    ```c++
    #include "ytest.h"
 
    // lambda function
    YTEST_CASE("A", []() {          // The name of this test case is "A"
        cout << "TEST A" << endl;
        return true;                // no error
    });

    // C-style function
    bool testB() {
        cout << "TEST B" << endl;
        return true;                // no error
    }
    YTEST_CASE("B", testB);         // The name of this test case is "B"

    // Function object
    struct TestC {
        bool operator()() const {
            cout << "TEST C" << endl;
            return true;
        }
    };
    YTEST_CASE("C", TestC());       // The name of this test case is "C"
    ```

  - C++ test suites
    ```c++
    #include "ytest.h"

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

        YEXPECT_TRUE(!3 != 0);
        YEXPECT_FALSE(!3 == 0);
        YEXPECT_EQ(!3, true);
        YEXPECT_NE(!3, false);        
    }
    ```

### Step3: add ```./src/ytest.cpp``` to your test projects

  - The following code can also be added to the main() function.

    ```c++
    #include "ytest.h"

    int main(int argc, char* argv[]) {
        //...
        return ytest::runTestCases(argc, argv);
    }
    ```

 ### Step4: compile!

 ### Step5: run test cases

  - run all test cases
    ```
	  $ <your-executable-file>
    ```

  - run specific test cases: a test case "A", a test case "B" and a test case "C"
    ```
    # <your-executable-file> A B C
    ```
    or
    ```
    # <your-executable-file> A,B,C
    ```
    or
    ```
    # <your-executable-file> A,B C
    ```

  - run specific test cases: all test cases to begin with "A" and a test case "C"
    ```
    # <your-executable-file> A*,C
    ```
    or
    ```
    # <your-executable-file> A* C
    ```
