#ifndef LIBSYM2OBJ_H_
#define LIBSYM2OBJ_H_

#include <algorithm>
#include <string>
#include <string_view>

#include "sym2obj/arg_list.h"

namespace sym2obj::lib {

std::string FindObjectFile(const sym2obj::ArgList &argv) {
  auto iter = std::find(argv.begin(), argv.end(), std::string_view{"-o"});
  if (iter == argv.end() || ++iter == argv.end() || *iter == nullptr) return "";
  return {*iter};
}

}  // namespace sym2obj::lib

#endif  // LIBSYM2OBJ_H_
