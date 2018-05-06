# young-test
Youngman's very simple test-framework

## How To Use

 - Step1: add ```./include``` to 'include' paths of your projects

 - Step2: add the test functions and function objects you want to call as shown below.
    ```
    // lambda function
    YTEST_ADD_CASE("A", []() {      // The name of this test case is "A"
        cout << "TEST A" << endl;
        return true;                // no error
    });
    ```

    ```
    // C-style function
    bool testB() {
        cout << "TEST B" << endl;
        return true;                // no error
    }

    YTEST_ADD_CASE("B", testB);     // The name of this test case is "B"
    ```

    ```
    // Function object
    struct TestC {
        bool operator()() const {
            cout << "TEST C" << endl;
            return true;
        }
    };

    YTEST_ADD_CASE("C", TestC());   // The name of this test case is "C"
    ```

 - Step3: add "ytest.cpp" to your test projects

 - Step4: compile

 - Step5: run tests
    - run all test cases
    ```
	# <your-executable-file>
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
