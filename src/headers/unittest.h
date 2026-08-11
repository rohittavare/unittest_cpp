#pragma once

#include <exception>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

namespace unittest {
    class unittest_assertion_failure: public logic_error {
        using logic_error::logic_error;
    };

    template <class T, class E = std::equal_to<T>>
    void assertEquals(const T& obj1, const T& obj2, const E& equal_op = E()) {
        if (!equal_op(obj1, obj2)) {
            throw unittest_assertion_failure("equality failed!");
        }
    }

    typedef struct TestCaseResult {
        string name;
        bool is_passed;
        string message;

        void print_result();
    } TestCaseResult;

    typedef struct TestSuiteResult {
        string name;
        unordered_set<string> passed_cases;
        unordered_map<string, string> failed_cases;

        int num_pass();
        int num_total();
        int num_fail();
        TestCaseResult get_test_result(string name);

        void print_results();
    } TestSuiteResult;

    class UnitTestSuite {
        private:
            string name;
            unordered_map<string, void (*)()> testcases;

        public:
            UnitTestSuite(string name);
            void addTest(string testcase_name, void (*testcase)());
            TestSuiteResult runTests();
    };
}
