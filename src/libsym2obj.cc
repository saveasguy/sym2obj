#include <dlfcn.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

#include "sym2obj/arg_list.h"
#include "sym2obj/preload.h"
#include "sym2obj/process.h"

namespace {

bool IsKnownCompiler(std::string_view compiler) {
  static const std::vector<std::string_view> kKnownCompilers = {
      "cc", "c++", "gcc", "g++", "clang", "clang++"};

  std::string compiler_name =
      std::filesystem::path(compiler).filename().string();
  auto iter =
      std::find(kKnownCompilers.begin(), kKnownCompilers.end(), compiler_name);
  return iter != kKnownCompilers.end();
}

void DoSym2Obj(std::string_view path, sym2obj::ArgList &argv,
               char *const envp[]) {
  if (!IsKnownCompiler(path)) return;

  auto object_file_path = sym2obj::FindObjectFile(argv);
  if (object_file_path.empty()) return;

  std::cout << object_file_path << std::endl;
}

void DoSym2Obj(std::string_view path, char *const argv[], char *const envp[]) {
  sym2obj::ArgList args{argv};
  DoSym2Obj(path, args, envp);
}

using ExecveTy = int (*)(const char *, char *const[], char *const[]);

int ExecveCallback(ExecveTy execve_impl, const char *path, char *const argv[],
                   char *const envp[]) {
  sym2obj::Process proc = sym2obj::RunProcess(execve_impl, path, argv, envp);
  int res = proc.Wait();
  if (res != 0) return res;
  DoSym2Obj(path, argv, envp);
  return res;
}

}  // namespace

PRELOAD_execve(ExecveCallback);
