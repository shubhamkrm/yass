#ifndef PARSER_H
#define PARSER_H

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "structs.h"

namespace yass {

class Parser {
 public:
  Parser(){};

  // Parses a content file and constructs a page struct. All elements of the
  // struct except page type can be empty. If no value for page type is
  // provided, the default type "post" is used.
  std::unique_ptr<Page> Parse(std::string &input);
  ~Parser(){};
};

}  // namespace yass

#endif /* PARSER_H */
