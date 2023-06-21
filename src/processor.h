#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <filesystem>
#include <string>
#include <vector>

#include "parser.h"
#include "theme.h"


namespace yass {
   
class Processor
{
private:
    std::vector<PostSummary> processed_posts_;
    Parser parser;
    Theme theme;

    void ProcessAssets(std::vector<std::filesystem::path> assets);
    void ProcessPages(std::vector<std::filesystem::path> pages);
    void ProcessPosts(std::vector<std::filesystem::path> posts);
public:
    Processor(Parser &parser, Theme &theme) : parser(parser), theme(theme) {}
    void ProcessContent();
    virtual ~Processor() {}
};

} /* yass */ 
#endif /* PROCESSOR_H */
