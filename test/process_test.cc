#include "sym2obj/process.h"

#include <gtest/gtest.h>

namespace {

void Foo(int a, int b) { std::exit(a + b); }

}  // namespace

TEST(ProcessSuite, ReturnCode) {
  sym2obj::Process proc = sym2obj::RunProcess(Foo, 1, 1);
  if (proc.running()) EXPECT_NE(proc.Wait(), 0);
}
