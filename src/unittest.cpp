#include <iostream>

#include "headers/unittest.h"

using namespace std;

namespace unittest {
    void TestCaseResult::print_result() {
        cout << this->name << ": " << (this->is_passed ? "PASS" : "FAIL") << endl;
        if (!this->is_passed) {
            cout << "message: " << this->message << endl;
        }
    }

    int TestSuiteResult::num_total() {
        return this->passed_cases.size() + this->failed_cases.size();
    }
    int TestSuiteResult::num_pass() {
        return this->passed_cases.size();
    }
    int TestSuiteResult::num_fail() {
        return this->failed_cases.size();
    }
    TestCaseResult TestSuiteResult::get_test_result(string name) {
        if (this->passed_cases.contains(name)) {
            return {
                .name = name,
                .is_passed = true
            };
        } else if (this->failed_cases.contains(name)) {
            return {
                .name = name,
                .is_passed = false,
                .message = this->failed_cases[name]
            };
        } else {
            throw invalid_argument("no testcase by the name " + name + " found in test suite " + this->name);
        }
    }
    void TestSuiteResult::print_results() {
        cout << "test suite " << this->name << " results:" << endl;
        cout << this->num_total() << " test cases run" << endl;
        cout << "---" << endl;
        if (this->num_fail()) {
            for (const auto& [n, m] : this->failed_cases) {
                this->get_test_result(n).print_result();
                cout << endl;
            }
            cout << this->num_pass() << " passed, " << this->num_fail() << " failed" << endl;
        } else {
            cout << "all tests passed!" << endl;
        }
    }

    UnitTestSuite::UnitTestSuite(string name):name(name) {}
    void UnitTestSuite::addTest(string testcase_name, void (*testcase)()) {
        this->testcases[testcase_name] = std::move(testcase);
    }
    TestSuiteResult UnitTestSuite::runTests() {
        unordered_set<string> passed_cases;
        unordered_map<string, string> failed_cases;
        for (const auto& [name, test_fn] : this->testcases) {
            try {
                test_fn();
                passed_cases.insert(name);
            } catch (unittest_assertion_failure& f) {
                failed_cases[name] = "assertion failure: " + string(f.what());
            } catch (exception& e) {
                failed_cases[name] = "uncaught exception: " + string(e.what());
            }
        }
        return {
            .name = this->name,
            .passed_cases = std::move(passed_cases),
            .failed_cases = std::move(failed_cases),
        };
    }
}