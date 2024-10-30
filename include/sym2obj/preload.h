#ifndef SYM2OBJ_PRELOAD_H_
#define SYM2OBJ_PRELOAD_H

#include <dlfcn.h>

#include <string_view>

#define PRELOAD(func, callback)                          \
  static_assert(std::is_function_v<decltype(callback)>); \
  PRELOAD_##func(callback)

namespace sym2obj::details {

template <typename T>
auto preload(std::string_view name) {
  return reinterpret_cast<T>(dlsym(RTLD_NEXT, name.data()));
}

}  // namespace sym2obj::details

#ifdef __cplusplus
extern "C" {
#endif

#define PRELOAD_execve(callback)                                              \
  int execve(const char *path, char *const argv[], char *const envp[]) {      \
    auto execve_impl = sym2obj::details::preload<decltype(&execve)>("execve"); \
    return callback(execve_impl, path, argv, envp);                           \
  }

#ifdef __cplusplus
}
#endif

#endif  // SYM2OBJ_PRELOAD_H
