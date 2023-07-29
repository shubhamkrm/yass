#include <fstream>
#include <sstream>
#include <string>

namespace yass {

// Reads the file at the input path and returns it as a string. The file must be
// a text file, and its contents must fit in memory.
std::string ReadFile(const std::filesystem::path path);
  
} /*  yass  */ 
