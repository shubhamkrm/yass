#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <filesystem>
#include <string>
#include <set>

#include "atom.h"
#include "parser.h"
#include "structs.h"
#include "theme.h"


namespace yass {
  
class Processor
{
private:
    std::set<PostSummary, PostSummaryCmp> processed_posts_;
    Parser parser;
    Theme theme;
    Atom atom;

    void ProcessAssets(std::vector<std::filesystem::path> assets);
    void ProcessPages(std::vector<std::filesystem::path> pages);
    void ProcessPosts(std::vector<std::filesystem::path> posts);
public:
    Processor(Parser &parser, Theme &theme, Atom &atom) :
        parser(parser), theme(theme), atom(atom) {}
    void ProcessContent();
    virtual ~Processor() {}
};

} /* yass */ 
#endif /* PROCESSOR_H */
