#pragma once

#include <string>
#include <functional>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <iostream>

///////////////////////////////////////////////////////////////////////////////
//  Macros & Constants

// for main()
#define YTEST_BEGIN                 int main(int argc, char* argv[]) {          \
                                        for (int i = 1; i < argc; i++)          \
                                            ytest::TestFramework::instance().addFilters(argv[i], ',');
#define YTEST_CASE(name, fn)            ytest::TestFramework::instance().add(name, fn);
#define YTEST_END                       ytest::TestFramework::instance().run(); \
                                        return 0;                               \
                                    }

#define _YTEST_CAT_(a,b)            a ## b
#define _YTEST_CAT(a,b)             _YTEST_CAT_(a,b)

#define YTEST_ADD_CASE(name, fn)    ytest::TestCaseAdder _YTEST_CAT(__adder_, __LINE__)(name, fn)

namespace ytest {

///////////////////////////////////////////////////////////////////////////////
//  Simple Test Framework

class TestFramework {
public:
    static TestFramework& instance() {
        static TestFramework test;
        return test;
    }

    void addCase(const std::string& name, const std::function<bool()>& fn) {
        testCases.emplace_back(name, fn);
    }

    void addFilters(const std::string& names, char delimiter) {
        int i, j = 0;
        for (i = 0; i < (int)names.length(); i++) {
            if (names[i] == delimiter) {
                if (j < i)
                    filters.insert(names.substr(j, i - j));
                j = i + 1;
            }
        }
        if (j < i)
            filters.insert(names.substr(j, i - j));
    }

    void addFilter(const std::string& name) {
        if (!name.empty())
            filters.insert(name);
    }

    void run() const {
        int ok = 0, failed = 0;
        for (auto& it : testCases) {
            if (filters.empty() || filters.find(it.first) != filters.end()) {
                if (it.second()) {
                    std::cout << "[" << it.first << "] " << "OK" << std::endl;
                    ok++;
                } else {
                    std::cout << "[" << it.first << "] " << "FAILED" << std::endl;
                    failed++;
                }
            }
        }

        std::cout << "TOTAL: " << (ok + failed) << ", SUCCESS: " << ok << ", FAILED: " << failed << std::endl;
    }

private:
    TestFramework() {
    }

    TestFramework(const TestFramework& rhs) = delete;
    TestFramework(TestFramework&& rhs) = delete;
    TestFramework& operator =(const TestFramework& rhs) = delete;
    TestFramework& operator =(TestFramework&& rhs) = delete;

private:
    std::vector<std::pair<std::string, std::function<bool()>>> testCases;
    std::unordered_set<std::string> filters;
};

struct TestCaseAdder {
    TestCaseAdder(const std::string& name, const std::function<bool()>& fn) {
        TestFramework::instance().addCase(name, fn);
    }
};

}