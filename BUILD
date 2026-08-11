load("@rules_cc//cc:cc_library.bzl", "cc_library")
load("@rules_cc//cc:cc_binary.bzl", "cc_binary")

cc_library(
    name = "unittest",
    srcs = [
        "unittest/unittest.cpp",
    ],
    hdrs = [
        "unittest/unittest.h",
    ],
)

cc_binary(
    name = "tests",
    srcs = [
        "tests/test_unittest.cpp",
    ],
    deps = [
        ":unittest",
    ],
)