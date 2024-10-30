#include "sym2obj/preload.h"

#include <gtest/gtest.h>

namespace {

int FooCallback() { return 42; }

#define PRELOAD_Foo(callback) \
  int Foo() { return callback(); }

PRELOAD(Foo, FooCallback)

int BarCallback(int a, int b) { return a + b; }

#define PRELOAD_Bar(callback) \
  int Bar(int a, int b) { return callback(a, b); }

PRELOAD(Bar, BarCallback)

void BazCallback() {}

#define PRELOAD_Baz(callback) \
  void Baz(void) { return callback(); }

PRELOAD(Baz, BazCallback)

}  // namespace

TEST(PreloadSuite, SimpleFunction) { EXPECT_EQ(Foo(), 42); }

TEST(PreloadSuite, FunctionWithArgs) { EXPECT_EQ(Bar(42, 42), 84); }

TEST(PreloadSuite, VoidFunction) { Baz(); }
