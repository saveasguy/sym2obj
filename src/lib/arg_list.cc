#include "sym2obj/arg_list.h"

#include <algorithm>

using namespace sym2obj;

sym2obj::ArgList::ArgList(char *const args[]) {
  for (; *args; ++args) args_.push_back(*args);
  args_.push_back(nullptr);
}

std::string sym2obj::FindObjectFile(const ArgList &argv) {
  auto iter = std::find(argv.begin(), argv.end(), std::string_view{"-o"});
  if (iter == argv.end() || ++iter == argv.end() || *iter == nullptr) return "";

  return {*iter};
}
