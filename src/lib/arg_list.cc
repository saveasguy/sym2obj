#include "sym2obj/arg_list.h"

#include <algorithm>

using namespace sym2obj;

sym2obj::ArgList::ArgList(char *const args[]) {
  for (; *args; ++args) args_.push_back(*args);
  args_.push_back(nullptr);
}
