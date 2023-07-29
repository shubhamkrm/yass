#include <filesystem>

#include "fileutils.h"

namespace yass {

std::string ReadFile(const std::filesystem::path path) {
  std::ifstream in(path);
  std::ostringstream sstr;
  sstr << in.rdbuf();
  return sstr.str();
}

}  // namespace yass
