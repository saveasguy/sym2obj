#include "sym2obj/process.h"

#include <sys/wait.h>

using namespace sym2obj;

int sym2obj::Process::Wait() noexcept {
  if (!running_) return 0;

  int exit_code = 0;
  int res = waitpid(pid_, &exit_code, 0);
  if (res == -1) return res;
  running_ = false;
  return WEXITSTATUS(exit_code);
}

char *GenEnvironmentVariable(std::string_view name) noexcept {
  return getenv(name.data());
}
