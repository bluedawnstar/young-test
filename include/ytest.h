#pragma once

#include <string>
#include <functional>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <iostream>

///////////////////////////////////////////////////////////////////////////////
//  Macros & Constants

#define YTEST_ASSERT(expression)    \
        do {                        \
            if(!(expression))       \
                std::cerr << "-- Assertion failed: " << #expression << ", " << __FILE__ << ", " << __LINE__ << std::endl; \
        } while (0)

#define _YTEST_CAT_(a,b)            a ## b
#define _YTEST_CAT(a,b)             _YTEST_CAT_(a,b)

#define YTEST_CASE(name, fn)        ytest::TestCaseAdder _YTEST_CAT(__adder_, __LINE__)(name, fn)

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
                    addFilter(names.substr(j, i - j));
                j = i + 1;
            }
        }
        if (j < i)
            addFilter(names.substr(j, i - j));
    }

    void addFilter(const std::string& name) {
        if (!name.empty()) {
            if (name.back() != '*')
                filters.insert(name);
            else
                prefixFilters.push_back(name.substr(0, name.length() - 1));
        }
    }

    void run() const {
        int ok = 0, failed = 0;
        for (auto& it : testCases) {
            if (canRun(it.first)) {
                if (it.second()) {
                    std::cout << "[" << it.first << "] " << "OK" << std::endl;
                    ok++;
                } else {
                    std::cout << "[" << it.first << "] " << "FAILED" << std::endl;
                    failed++;
                }
            }
        }

        std::cout << std::endl << "TOTAL: " << (ok + failed) << ", SUCCESS: " << ok << ", FAILED: " << failed << std::endl;
    }

private:
    TestFramework() {
    }

    TestFramework(const TestFramework& rhs) = delete;
    TestFramework(TestFramework&& rhs) = delete;
    TestFramework& operator =(const TestFramework& rhs) = delete;
    TestFramework& operator =(TestFramework&& rhs) = delete;

    bool canRun(const std::string& name) const {
        if (filters.empty() && prefixFilters.empty())
            return true;

        if (!filters.empty()) {
            if (filters.find(name) != filters.end())
                return true;
        }
        if (!prefixFilters.empty()) {
            for (auto& s : prefixFilters) {
                auto len = s.length();
                if (len <= name.length() && name.compare(0, len, s, 0, len) == 0)
                    return true;
            }
        }

        return false;
    }

private:
    std::vector<std::pair<std::string, std::function<bool()>>> testCases;
    std::unordered_set<std::string> filters;
    std::vector<std::string> prefixFilters;
};

struct TestCaseAdder {
    TestCaseAdder(const std::string& name, const std::function<bool()>& fn) {
        TestFramework::instance().addCase(name, fn);
    }
};

}
