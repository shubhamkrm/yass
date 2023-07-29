#ifndef STRUCTS_H
#define STRUCTS_H 

#include <any>
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
    std::map<std::string, std::any> metadata;
    std::string content;
    fs::path path;
    PostSummary prev;
    PostSummary next;

    PostSummary GetPostSummary() {
        return {
            .path = path,
                // TODO: Error checking
            .title = std::any_cast<std::string>(metadata["title"]),
            .date = std::any_cast<std::string>(metadata["date"]),
        };
    }
};

} /* yass  */ 

#endif /* STRUCTS_H */
