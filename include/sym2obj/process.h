#ifndef SYM2OBJ_SUPPORT_H_
#define SYM2OBJ_SUPPORT_H_

#include <sys/types.h>
#include <unistd.h>

#include <cstdlib>
#include <tuple>
#include <type_traits>
#include <utility>

namespace sym2obj {

using PID = pid_t;

class Process {
  template <typename FnTy, class... FnArgsTy>
  friend Process RunProcess(FnTy &&, FnArgsTy &&...);

 public:
  /// @brief Wait the process to end, if it is running.
  /// @return Return code of the running process, or 0 if not proccess was run.
  int wait();

  bool running() { return running_; }

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
  auto launcher = [fn2 = std::forward<FnTy>(fn),
                   args2 = std::move(args_wrapper)](bool &running_) -> PID {
    pid_t pid = fork();
    switch (pid) {
      case -1: {
        running_ = false;
        return pid;
      }
      case 0: {
        std::apply(std::forward<decltype(fn2)>(fn2),
                   std::forward<decltype(args2)>(args2));
        std::exit(0);
      }
    }
    return pid;
  };

  return Process{launcher};
}

}  // namespace sym2obj

#endif  // SYM2OBJ_SUPPORT_H_
