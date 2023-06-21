#ifndef PARSER_H
#define PARSER_H

#include <map>
#include <memory>
#include <string>
#include <vector>

namespace yass
{

struct PostSummary {
    std::string path;
    std::string title;
    std::string date;
};
 
struct Page {
    std::string type;
    std::map<std::string, std::string> metadata;
    std::string content;
};

struct IndexPage : public Page {
    std::vector<PostSummary> post_summaries;
};

class Parser
{
private:
    

public:
    Parser() {};
    std::unique_ptr<Page> Parse(std::string &input);
    virtual ~Parser() {};
};

} /* yass */ 

#endif /* PARSER_H */
