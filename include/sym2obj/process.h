#ifndef SYM2OBJ_SUPPORT_H_
#define SYM2OBJ_SUPPORT_H_

#include <cstdlib>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <utility>

#include <sys/types.h>
#include <unistd.h>

namespace sym2obj {

using PID = pid_t;

class Process final {
  template <typename FnTy, class... FnArgsTy>
  friend Process RunProcess(FnTy &&, FnArgsTy &&...);

 public:
  /// @brief Wait the process to end, if it is running.
  /// @return Return code of the running process, or 0 if not proccess was run.
  int Wait() noexcept;

  bool running() const noexcept { return running_; }

 private:
  template <typename FnTy, typename = std::enable_if_t<
                               std::is_invocable_r_v<PID, FnTy, bool &>>>
  explicit Process(FnTy &&fn) {
    running_ = false;
    pid_ = fn(running_);
  }

  PID pid_;
  bool running_;
};

/// @brief Run the function in a different process.
/// @tparam FuncTy
/// @param func
/// @return Return a process instance.
template <typename FnTy, class... FnArgsTy>
Process RunProcess(FnTy &&fn, FnArgsTy &&...args) {
  auto args_wrapper = std::make_tuple(std::forward<FnArgsTy>(args)...);
  auto launcher = [&fn, &args_wrapper](bool &running_) -> PID {
    pid_t pid = fork();
    switch (pid) {
      case -1: {
        running_ = false;
        return pid;
      }
      case 0: {
        std::apply(std::forward<FnTy>(fn), std::move(args_wrapper));
        std::exit(0);
      }
    }
    running_ = true;
    return pid;
  };

  return Process{launcher};
}

char *GenEnvironmentVariable(std::string_view name) noexcept;

}  // namespace sym2obj

#endif  // SYM2OBJ_SUPPORT_H_
