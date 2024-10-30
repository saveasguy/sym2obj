#include <gtest/gtest.h>

#include <sym2obj/preload.h>

namespace {

int FooCallback() { return 42; }

#define PRELOAD_foo(callback) \
int foo() { return callback(); }

PRELOAD(foo, FooCallback)


int BarCallback(int a, int b) { return a + b; }

#define PRELOAD_bar(callback) \
int bar(int a, int b) { return callback(a, b); }

PRELOAD(bar, BarCallback)


void BazCallback() { }

#define PRELOAD_baz(callback) \
void baz(void) { return callback(); }

PRELOAD(baz, BazCallback)

}

TEST(PreloadSuite, SimpleFunction) {
    EXPECT_EQ(foo(), 42);
}

TEST(PreloadSuite, FunctionWithArgs) {
    EXPECT_EQ(bar(42, 42), 84);
}

TEST(PreloadSuite, VoidFunction) {
    baz();
}
