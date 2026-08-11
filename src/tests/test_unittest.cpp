#include <stdexcept>
#include "src/headers/unittest.h"

using namespace std;

// functions used for exercising the unittest utility

void test_no_op() {}

void test_throw_exception() {
    throw runtime_error("intentionally thrown!");
}

void test_assert_pass() {
    int expected = 55;
    int actual = 0;
    for (int i = 1; i <= 10; i++) {
        actual+=i;
    }
    unittest::assertEquals(actual, expected);
}

void test_assert_fail() {
    int expected = 55;
    int actual = 0;
    for (int i = 0; i < 10; i++) {
        actual+=1;
    }
    unittest::assertEquals(actual, expected);
}

// helper object to compare two unittest results
template<>
struct std::equal_to<unittest::TestCaseResult> {
    bool operator()(const unittest::TestCaseResult& a, const unittest::TestCaseResult& b) const {
        return (
            a.name == b.name
            && a.is_passed == b.is_passed
            && a.message == b.message
        );
    }
};

// helper to run a simple scenario & validate output
void run_single_test_case_scenario(string scenario_name, string case_name, void (*test_fn)(), unittest::TestCaseResult expected) {
    unittest::UnitTestSuite t(scenario_name);
    t.addTest(case_name, test_fn);
    auto output = t.runTests();

    unittest::assertEquals(output.num_total(), 1);
    unittest::assertEquals(output.num_fail(), (expected.is_passed) ? 0 : 1);
    unittest::assertEquals(output.num_pass(), (expected.is_passed) ? 1 : 0);

    auto actual = output.get_test_result(case_name);
    unittest::assertEquals(actual, expected);
}

// the actual test cases

void test_unittest_no_op() {
    unittest::UnitTestSuite t("test_unittest_no_op");
    auto output = t.runTests();

    unittest::assertEquals(output.num_total(), 0);
    unittest::assertEquals(output.num_fail(), 0);
    unittest::assertEquals(output.num_pass(), 0);
    unittest::assertEquals(output.name, string("test_unittest_no_op"));
}

void test_unittest_single_test_pass() {
    unittest::TestCaseResult expected("test_no_op", true);
    run_single_test_case_scenario("test_unittest_single_test_pass", "test_no_op", &test_no_op, expected);
}

void test_unittest_uncaught_exception() {
    unittest::TestCaseResult expected("test_throw_exception", false, "uncaught exception: intentionally thrown!");
    run_single_test_case_scenario("test_unittest_uncaught_exception", "test_throw_exception", &test_throw_exception, expected);
}

void test_unittest_assertion_pass() {
    unittest::TestCaseResult expected("test_assert_pass", true);
    run_single_test_case_scenario("test_unittest_assertion_pass", "test_assert_pass", &test_assert_pass, expected);
}

void test_unittest_assertion_fail() {
    unittest::TestCaseResult expected("test_assert_fail", false, "assertion failure: equality failed!");
    run_single_test_case_scenario("test_unittest_assertion_fail", "test_assert_fail", &test_assert_fail, expected);
}

void test_unittest_multiple() {
    unittest::UnitTestSuite t("test_unittest_multiple");
    t.addTest("test_no_op", &test_no_op);
    t.addTest("test_assert_pass", &test_assert_pass);
    auto output = t.runTests();

    unittest::assertEquals(output.num_total(), 2);
    unittest::assertEquals(output.num_fail(), 0);
    unittest::assertEquals(output.num_pass(), 2);

    unittest::assertEquals(output.get_test_result("test_no_op"), unittest::TestCaseResult("test_no_op", true));
    unittest::assertEquals(output.get_test_result("test_assert_pass"), unittest::TestCaseResult("test_assert_pass", true));
}

void test_unittest_multiple_with_failure() {
    unittest::UnitTestSuite t("test_unittest_multiple");
    t.addTest("test_assert_fail", &test_assert_fail);
    t.addTest("test_no_op", &test_no_op);
    auto output = t.runTests();

    unittest::assertEquals(output.num_total(), 2);
    unittest::assertEquals(output.num_fail(), 1);
    unittest::assertEquals(output.num_pass(), 1);

    unittest::assertEquals(output.get_test_result("test_no_op"), unittest::TestCaseResult("test_no_op", true));
    unittest::assertEquals(output.get_test_result("test_assert_fail"), unittest::TestCaseResult("test_assert_fail", false, "assertion failure: equality failed!"));
}

int main() {
    unittest::UnitTestSuite uts("test_unittest");
    uts.addTest("test_unittest_no_op", &test_unittest_no_op);
    uts.addTest("test_unittest_single_test_pass", &test_unittest_single_test_pass);
    uts.addTest("test_unittest_uncaught_exception", &test_unittest_uncaught_exception);
    uts.addTest("test_unittest_assertion_pass", &test_unittest_assertion_pass);
    uts.addTest("test_unittest_assertion_fail", &test_unittest_assertion_fail);
    uts.addTest("test_unittest_multiple", &test_unittest_multiple);
    uts.addTest("test_unittest_multiple_with_failure", &test_unittest_multiple_with_failure);

    // run & output results
    auto results = uts.runTests();
    results.print_results();
    return results.num_fail() > 0;
}