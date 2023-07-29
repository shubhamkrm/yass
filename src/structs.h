#ifndef STRUCTS_H
#define STRUCTS_H

#include <any>
#include <filesystem>
#include <map>
#include <memory>
#include <string>

namespace yass {
namespace fs = std::filesystem;

// Summary details for blog posts. Useful for creating indexes.
struct PostSummary {
  // Relative path of the post.
  std::string path;

  // Title of the blog post.
  std::string title;

  // Date of the blog post in %Y-%m-%d format.
  std::string date;
};

// Comparator for sorting post summaries in a reverse chronological order.
struct PostSummaryCmp {
  bool operator()(const PostSummary& lhs, const PostSummary& rhs) const {
    return lhs.date > rhs.date;
  }
};

// Represents a parsed page.
struct Page {
  // Type of page, such as 'post', 'index', etc.
  std::string type;

  // Metadata derived from the frontmatter. The frontmatter currently supports
  // string and lists as metadata value types.
  std::map<std::string, std::any> metadata;

  // Parsed markdown content in HTML format.
  std::string content;

  // Relative path of the page.
  fs::path path;

  // Summary of the previous post in a reverse chronological order.
  PostSummary prev;
  // Summary of the next post in a reverse chronological order.
  PostSummary next;

  // Helper function which constructs and returns a post summary for the post.
  // Maybe I can optimize it further by storing references and avoiding copies.
  PostSummary GetPostSummary() {
    return {
        .path = path,
        // TODO: Error checking
        .title = std::any_cast<std::string>(metadata["title"]),
        .date = std::any_cast<std::string>(metadata["date"]),
    };
  }
};

}  // namespace yass

#endif /* STRUCTS_H */
