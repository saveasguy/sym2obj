#include "internal/compiler_parser.h"
#include "sym2obj/arg_list.h"
#include "sym2obj/preload.h"
#include "sym2obj/process.h"

#include <algorithm>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <string>
#include <string_view>
#include <vector>

#include <dlfcn.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <elfio/elfio.hpp>

using namespace sym2obj::lib;

namespace {

const char kSym2objDirEnv[] = "SYM2OBJ_DIR";

std::vector<std::string> GetSymbolsFromObjectFile(std::istream &obj) {
  std::vector<std::string> symbols;
  ELFIO::elfio elf;
  if (!elf.load(obj)) return {};
  for (auto &&section : elf.sections) {
    ELFIO::symbol_section_accessor symbols_accessor{elf, section.get()};
    for (int i = 0; i < symbols_accessor.get_symbols_num(); ++i) {
      std::string name;
      ELFIO::Elf64_Addr value;
      ELFIO::Elf_Xword size;
      unsigned char bind;
      unsigned char type;
      ELFIO::Elf_Half section_index;
      unsigned char other;
      symbols_accessor.get_symbol(i, name, value, size, bind, type,
                                  section_index, other);
      if (type == ELFIO::STT_FUNC || type == ELFIO::STT_OBJECT)
        symbols.push_back(name);
    }
  }
  return symbols;
}

bool MakeUniqueFile(std::string &template_path) {
  int fd = mkstemp(template_path.data());
  if (fd == -1) return false;
  close(fd);
  return true;
}

void DumpAsJSONArray(std::ostream &out,
                     const std::vector<std::string> &strings) {
  if (strings.empty()) {
    out << "[]";
    return;
  }
  out << "[";
  std::transform(strings.begin(), strings.end() - 1,
                 std::ostream_iterator<std::string>(out, ","),
                 [](const std::string &str) { return "\"" + str + "\""; });
  out << "\"" << strings.back() << "\"";
  out << "]";
}

void DumpObjectToSymbolsMapping(const std::filesystem::path &object,
                                const std::vector<std::string> &symbols) {
  char *sym2obj_dir = getenv(kSym2objDirEnv);
  if (!sym2obj_dir) return;
  auto unique_file =
      std::filesystem::absolute(sym2obj_dir).string() + "/XXXXXXXX";
  if (!MakeUniqueFile(unique_file)) return;
  std::ofstream out{unique_file};
  if (!out) return;
  out << "[\"" << std::filesystem::absolute(object).string() << "\",";
  DumpAsJSONArray(out, symbols);
  out << "]";
}

void DoSym2Obj(std::string_view path, const sym2obj::ArgList &argv) {
  if (!IsKnownCompiler(path)) return;

  auto obj_path = FindObjectFile(argv);
  std::ifstream obj{obj_path, std::ios::binary};
  if (!obj) return;

  auto symbols = GetSymbolsFromObjectFile(obj);
  if (symbols.empty()) return;

  DumpObjectToSymbolsMapping(obj_path, symbols);
}

void DoSym2Obj(std::string_view path, char *const argv[]) {
  sym2obj::ArgList args{argv};
  DoSym2Obj(path, args);
}

using ExecveTy = int (*)(const char *, char *const[], char *const[]);

int ExecveCallback(ExecveTy execve_impl, const char *path, char *const argv[],
                   char *const envp[]) {
  try {
    sym2obj::Process proc = sym2obj::RunProcess(execve_impl, path, argv, envp);
    if (!proc.running()) {
      std::cout << "whjart" << std::endl;
      errno = EACCES;
      return -1;
    }
    int res = proc.Wait();
    if (res != 0) std::exit(res);
    DoSym2Obj(path, argv);
    std::exit(res);
  } catch (const std::exception &e) {
    std::cerr << "libsym2obj.so: WARNING: unhandled exception with the reason: "
              << e.what() << std::endl;
  } catch (...) {
    std::cerr << "libsym2obj.so: WARNING: unhandled exception" << std::endl;
  }
  return execve_impl(path, argv, envp);
}

}  // namespace

PRELOAD_execve(ExecveCallback);
