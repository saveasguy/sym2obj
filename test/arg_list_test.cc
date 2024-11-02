#include "internal/compiler_parser.h"

#include <gtest/gtest.h>

TEST(ArgListSuite, FindObjectFile) {
  std::string gcc = "gcc", o = "-o", obj = "obj.o";
  char *const argv[] = {gcc.data(), o.data(), obj.data()};
  sym2obj::ArgList argv_list{argv};
  EXPECT_EQ(sym2obj::lib::FindObjectFile(argv), "obj.o");
}
