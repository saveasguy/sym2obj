#ifndef INTERNAL_COMPILER_PARSER_H_
#define INTERNAL_COMPILER_PARSER_H_

#include "sym2obj/arg_list.h"

#include <algorithm>
#include <filesystem>
#include <string_view>
#include <vector>

namespace sym2obj::lib {

inline const std::vector<std::string_view> kKnownCompilers = {
    "cc", "c++", "gcc", "g++", "clang", "clang++"};

inline bool IsKnownCompiler(std::string_view compiler) {
  std::string compiler_name =
      std::filesystem::path(compiler).filename().string();
  auto iter =
      std::find(kKnownCompilers.begin(), kKnownCompilers.end(), compiler_name);
  return iter != kKnownCompilers.end();
}

inline std::filesystem::path FindObjectFile(
    const sym2obj::ArgList &argv) {
  auto iter = std::find(argv.begin(), argv.end(), std::string_view{"-o"});
  if (iter == argv.end() || ++iter == argv.end() || *iter == nullptr) return "";
  return {*iter};
}

}  // namespace sym2obj::lib

#endif
