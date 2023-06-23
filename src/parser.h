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
    Parser() {};
    std::unique_ptr<Page> Parse(std::string &input);
    virtual ~Parser() {};
};

} /* yass */ 

#endif /* PARSER_H */
