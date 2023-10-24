#pragma once

#ifndef __YTEST_H__
#define __YTEST_H__

#include <chrono>
#include <string>
#include <functional>
#include <vector>
#include <unordered_map>
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

#define _YTEST_CAT_(a,b)                    a ## b
#define _YTEST_CAT(a,b)                     _YTEST_CAT_(a,b)

#define YTEST_CASE(name, fn)                static ytest::TestCaseAdder _YTEST_CAT(__adder_, __LINE__)(name, fn)
#define YTEST_CASE2(tester, caseName, fn)   static ytest::TestCaseAdder _YTEST_CAT(__adder_, __LINE__)(tester, caseName, fn)

namespace ytest {

struct TestContext {
    bool isFirst = false;
    bool isLast = false;
};

///////////////////////////////////////////////////////////////////////////////
//  Elapsed Timer

struct ElapsedTimer {
    ElapsedTimer() {
        resetTotal();
    }

    void resetGroup() {
        groupSum = 0;
    }

    void resetTotal() {
        groupSum = 0;
        totalSum = 0;
    }

    void start() {
        beg = std::chrono::high_resolution_clock::now();
    }

    long long elapsedNanosec() {
        auto res = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - beg).count();
        groupSum += res;
        totalSum += res;
        return res;
    }

    long long elapsedMillisec() {
        return elapsedNanosec() / 1000000;
    }


    long long getGroupElapsedNanosec() const {
        return groupSum;
    }

    long long getTotalElapsedNanosec() const {
        return totalSum;
    }

    long long getGroupElapsedMillisec() const {
        return groupSum / 1000000;
    }

    long long getTotalElapsedMillisec() const {
        return totalSum / 1000000;
    }

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> beg;
    long long groupSum = 0;
    long long totalSum = 0;
};


///////////////////////////////////////////////////////////////////////////////
//  Test Suite

class TestSuite {
public:
    TestSuite() {
    }

    virtual ~TestSuite() {
    }

    static void setUpTestSuite() {
    }

    static void tearDownTestSuite() {
    }

    virtual void setUp() {
    }

    virtual void tearDown() {
    }

protected:
    virtual void __testBody() = 0;

    bool __runCase() {
        setUp();
        __setResult(true);
        __testBody();
        tearDown();
        return __result;
    }

    void __setResult(bool res) {
        __result = res;
    }

private:
    bool __result = true;
};


///////////////////////////////////////////////////////////////////////////////
//  Simple Test Framework

class TestFramework {
public:
    static TestFramework& instance() {
        static TestFramework test;
        return test;
    }

    void addCase(const std::string& testerName, const std::string& caseName, const std::function<bool(const TestContext&)>& fn) {
        testSuites[testerName].emplace_back(caseName, fn);
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

    int getTotalCaseCount() const {
        int res = 0;
        for (auto& vec : testSuites)
            res += static_cast<int>(vec.second.size());
        return res;
    }

    int run() const {
        #define YTEST_ATTR_GREEN(s) ("\x1b[32m" s "\x1b[0m")
        #define YTEST_ATTR_RED(s)   ("\x1b[31m" s "\x1b[0m")

        std::cout << YTEST_ATTR_GREEN("[==========]") << " Running " << makeTestCountString(getTotalCaseCount())
                  << " from " << testSuites.size() << " test suites" << std::endl
                  << std::endl;
        ElapsedTimer timer;

        int ok = 0, failed = 0, skipped = 0;
        for (auto& ts : testSuites) {
            int groupCaseCount = static_cast<int>(ts.second.size());
            auto testName = std::string(YTEST_ATTR_GREEN("[----------]"))
                          + " " + makeTestCountString(groupCaseCount)
                          + " from " + (ts.first.empty() ? "global context" : ts.first);

            timer.resetGroup();

            std::cout << testName << std::endl;
            for (int i = 0; i < static_cast<int>(ts.second.size()); i++) {
                auto& it = ts.second[i];
                auto name = ts.first + "." + it.first;

                TestContext ctx;
                ctx.isFirst = (i == 0);
                ctx.isLast = (i + 1 >= static_cast<int>(ts.second.size()));

                std::cout << YTEST_ATTR_GREEN("[ RUN      ]") << " " << name << std::endl;

                if (canRun(name)) {
                    timer.start();
                    bool success = it.second(ctx);
                    auto elapsed = timer.elapsedMillisec();

                    auto* msg = "";
                    if (success) {
                        msg = YTEST_ATTR_GREEN("[       OK ]");
                        ok++;
                    } else {
                        msg = YTEST_ATTR_RED("[   FAILED ]");
                        failed++;
                    }
                    std::cout << msg << " " << name << " (" << elapsed << " ms)" << std::endl;
                } else {
                    skipped++;
                }
            }
            std::cout << testName << " (" << timer.getGroupElapsedMillisec() << " ms)" << std::endl
                      << std::endl;
        }

        std::cout << YTEST_ATTR_GREEN("[==========]") << " " << makeTestCountString(getTotalCaseCount()) << " (" << timer.getTotalElapsedMillisec() << " ms)" << std::endl;
        if (ok > 0)
            std::cout << YTEST_ATTR_GREEN("[  PASSED  ]") << " " << ok << std::endl;
        if (failed > 0)
            std::cout << YTEST_ATTR_RED("[  FAILED  ]") << " " << failed << std::endl;
        if (skipped > 0)
            std::cout << "[ SKIPPED  ] " << skipped << std::endl;

        #undef YTEST_ATTR_GREEN
        #undef YTEST_ATTR_RED

        return 0;
    }

    int run(int argc, char* argv[]) {
        for (int i = 1; i < argc; i++)
            addFilters(argv[i], ',');

        return run();
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

    static std::string makeTestCountString(int cnt) {
        std::string res = std::to_string(cnt) + " test";
        if (cnt > 1)
            res.push_back('s');
        return res;
    }

private:
    std::unordered_map<std::string, std::vector<std::pair<std::string, std::function<bool(const TestContext&)>>>> testSuites;
    std::unordered_set<std::string> filters;
    std::vector<std::string> prefixFilters;
};

struct TestCaseAdder {
    TestCaseAdder(const std::string& name, const std::function<bool(const TestContext&)>& fn) {
        TestFramework::instance().addCase("", name, fn);
    }

    TestCaseAdder(const std::string& testerName, const std::string& caseName, const std::function<bool(const TestContext&)>& fn) {
        TestFramework::instance().addCase(testerName, caseName, fn);
    }
};


inline int runTestCases(int argc, char* argv[]) {
    return TestFramework::instance().run(argc, argv);
}


//--- print value

template<typename T>
inline std::ostream& printValue(std::ostream& os, const T& val) {
    return os << val;
}

template<>
inline std::ostream& printValue(std::ostream& os, const bool& val) {
    return os << (val ? "true" : "false");
}

template<typename T, typename U>
inline std::ostream& printComparedValues(std::ostream& os, const T& val1, const U& val2) {
    printValue(os, val1);
    os << " vs ";
    printValue(os, val2);
    return os;
}

} // namespace ytest


/*
TODO: assertion and return result!
*/
#define YTEST_F(__tester, __caseName)                                   \
    class __tester##__caseName : public __tester {                      \
    public:                                                             \
        bool operator()(const ytest::TestContext& ctx) {                \
            if (ctx.isFirst)                                            \
                setUpTestSuite();                                       \
            bool res = __runCase();                                     \
            if (ctx.isLast)                                             \
                tearDownTestSuite();                                    \
            return res;                                                 \
        }                                                               \
                                                                        \
    protected:                                                          \
        virtual void __testBody() override;                             \
    };                                                                  \
                                                                        \
    YTEST_CASE2(#__tester, #__caseName, __tester##__caseName());        \
    void __tester##__caseName::__testBody()

#define YEXPECT_TRUE(expr)                                              \
    do {                                                                \
        if (!(expr)) {                                                  \
            std::cerr << __FILE__ << ":" << __LINE__ << ": Failure" << std::endl; \
            std::cerr << "Value of: " << #expr << std::endl;            \
            std::cerr << "  Actual: false" << std::endl;                \
            std::cerr << "Expected: true" << std::endl << std::endl;    \
            __setResult(false);                                         \
        }                                                               \
    } while (0)

#define YEXPECT_FALSE(expr)                                             \
    do {                                                                \
        if ((expr)) {                                                   \
            std::cerr << __FILE__ << ":" << __LINE__ << ": Failure" << std::endl; \
            std::cerr << "Value of: " << #expr << std::endl;            \
            std::cerr << "  Actual: true" << std::endl;                 \
            std::cerr << "Expected: false" << std::endl << std::endl;   \
            __setResult(false);                                         \
        }                                                               \
    } while (0)

#define YEXPECT_EQ(expr1, expr2)                                        \
    do {                                                                \
        auto val1 = (expr1);                                            \
        auto val2 = (expr2);                                            \
        if (val1 != val2) {                                             \
            std::cerr << __FILE__ << ":" << __LINE__ << ": Failure" << std::endl; \
            std::cerr << "Expected: (" << #expr1 << ") == (" << #expr2 << ")" << std::endl; \
            std::cerr << "  Actual: "; ytest::printComparedValues(std::cerr, val1, val2); \
            std::cerr << std::endl << std::endl;                        \
            __setResult(false);                                         \
        }                                                               \
    } while (0)

#define YEXPECT_NE(expr1, expr2)                                        \
    do {                                                                \
        auto val1 = (expr1);                                            \
        auto val2 = (expr2);                                            \
        if (val1 == val2) {                                             \
            std::cerr << __FILE__ << ":" << __LINE__ << ": Failure" << std::endl; \
            std::cerr << "Expected: (" << #expr1 << ") != (" << #expr2 << ")" << std::endl; \
            std::cerr << "  Actual: "; ytest::printComparedValues(std::cerr, val1, val2); \
            std::cerr << std::endl << std::endl;                        \
            __setResult(false);                                         \
        }                                                               \
    } while (0)


/*
#include "ytest.h"

int main(int argc, char* argv[]) {
    return ytest::runTestCases(argc, argv);
}
*/


#endif // !__YTEST_H__
