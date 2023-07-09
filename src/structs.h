#ifndef STRUCTS_H
#define STRUCTS_H 

#include <filesystem>
#include <memory>
#include <map>
#include <string>

namespace yass {
namespace fs = std::filesystem;

struct PostSummary {
    std::string path;
    std::string title;
    std::string date;
};

struct PostSummaryCmp {
    bool operator()(const PostSummary& lhs, const PostSummary& rhs) const {
        return lhs.date > rhs.date;
    }
};
    
struct Page {
    std::string type;
    std::map<std::string, std::string> metadata;
    std::string content;
    fs::path path;
    PostSummary prev;
    PostSummary next;

    PostSummary GetPostSummary() {
        return {
            .path = path,
            .title = metadata["title"],
            .date = metadata["date"]
        };
    }
};

} /* yass  */ 

#endif /* STRUCTS_H */
