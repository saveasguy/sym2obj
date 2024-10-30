#include "sym2obj/process.h"

#include <sys/wait.h>

using namespace sym2obj;

int sym2obj::Process::wait() {
  if (!running_) return 0;
  // For now, we just ignore errors of waitpid
  int res = waitpid(pid_, nullptr, 0);
  running_ = false;
  return res;
}
