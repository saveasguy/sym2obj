#ifndef SYM2OBJ_ARG_LIST_H_
#define SYM2OBJ_ARG_LIST_H_

#include <list>
#include <string>
#include <vector>

namespace sym2obj {

class ArgList {
 public:
  using iterator = std::vector<char *>::iterator;
  using const_iterator = std::vector<char *>::const_iterator;

 public:
  ArgList(char *const args[]);

  char *const *get_args() const noexcept { return args_.data(); }

  iterator begin() noexcept { return args_.begin(); }
  iterator end() noexcept { return args_.end(); }
  const_iterator begin() const noexcept { return args_.begin(); }
  const_iterator end() const noexcept { return args_.end(); }

 private:
  std::vector<char *> args_;
};

/// @brief Find path to produced object file in compiler arguments.
/// @param argv Compiler arguments.
/// @return Path to object file or empy string, if not found.
std::string FindObjectFile(const ArgList &argv);

}  // namespace sym2obj

#endif  // SYM2OBJ_ARG_LIST_H_
