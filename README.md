# unittest

A minimal C++20 unit testing library. Group test functions into a
`UnitTestSuite`, run them, and get pass/fail results with captured
assertion failures and uncaught exceptions.

## Usage

```cpp
#include "unittest/unittest.h"

void test_addition() {
    unittest::assertEquals(2 + 2, 4);
}

int main() {
    unittest::UnitTestSuite suite("my_suite");
    suite.addTest("test_addition", &test_addition);

    auto results = suite.runTests();
    results.print_results();
    return results.num_fail() > 0;
}
```

- `unittest::assertEquals(actual, expected)` throws `unittest_assertion_failure`
  on mismatch; any other uncaught exception in a test is also recorded as a
  failure.
- `UnitTestSuite::runTests()` returns a `TestSuiteResult` with `num_pass()`,
  `num_fail()`, `num_total()`, and `get_test_result(name)` for inspecting
  individual cases.

### Using this library from another Bazel repo

The library target is `//:unittest` and is public. In the consuming repo's
`MODULE.bazel`, add a `bazel_dep` on `unittest` plus an override pointing at
this repo, e.g. via `git_override`:

```python
# MODULE.bazel (consumer)
bazel_dep(name = "unittest", version = "0.0.0")
git_override(
    module_name = "unittest",
    remote = "https://github.com/rohittavare/unittest_cpp",
)
```

or, for local development, `local_path_override`:

```python
# MODULE.bazel (consumer)
bazel_dep(name = "unittest", version = "0.0.0")
local_path_override(
    module_name = "unittest",
    path = "/path/to/unittest",
)
```

Then depend on `@unittest//:unittest` from a `BUILD` target:

```python
# BUILD (consumer)
cc_binary(
    name = "my_app",
    srcs = ["main.cpp"],
    deps = ["@unittest//:unittest"],
)
```

## Features & limitations

**Features**
- Register free-function test cases with `addTest(name, fn)` and run them all
  via `runTests()`.
- Per-test isolation: an assertion failure or any uncaught exception in one
  test is caught and recorded without stopping the rest of the suite.
- `assertEquals` accepts a custom equality functor (`E`), so it works with
  types that don't define `operator==` or where you want different equality
  semantics.
- Structured results (`TestSuiteResult`/`TestCaseResult`) let you query
  pass/fail counts and per-test outcomes programmatically, not just via
  printed output.
- Public Bazel target (`//:unittest`) intended for cross-repo reuse.

**Limitations**
- Test cases must be plain `void (*)()` function pointers — no lambda
  captures, member functions, or arguments.
- Only one assertion helper (`assertEquals`); no `assertTrue`, `assertThrows`,
  `assertNotEquals`, etc.
- No setup/teardown (fixtures), parameterized tests, or nested suites.
- Tests are stored in an `unordered_map`, so execution order is unspecified;
  registering two tests with the same name silently overwrites the first.
- No test discovery — every test must be registered manually in `main()`.
- No timeouts, and execution is single-threaded/sequential.
- No CLI (filtering by name/pattern, verbosity, etc.).

## Running tests

This project builds with [Bazel](https://bazel.build/) (via `bzlmod`,
`rules_cc`) and requires a C++20 compiler.

```sh
bazel build //...
bazel run //:tests
```

`//:tests` builds `tests/test_unittest.cpp`, which exercises the library
against itself, and exits non-zero if any test case fails.
