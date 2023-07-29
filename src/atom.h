#ifndef ATOM_H
#define ATOM_H

#include <chrono>
#include <filesystem>
#include <string>
#include <vector>

#include "config.h"

namespace yass {

// Handles generation and write for Atom feeds for the website.
class Atom {
 public:
  // Represents a single feed entry in the Atom feed.
  struct Entry {
    // Title of the feed article.
    std::string title;
    // Canonical URL of the article. Please provide absolute URL to the blog
    // entry here.
    std::string url;
    // Published time as a UNIX epoch timestamp.
    time_t published_time;
    // Update time as a UNIX epoch timestamp.
    time_t update_time;
    // Content of the article as HTML. The caller is free to either provide
    // the full article, or an excerpt.
    std::string content;
  };

  Atom(const SiteConfig& site_config);

  // Adds an entry to the atom file in an ordered manner.
  void AddEntry(const Entry& entry);

  // Writes all entries to a feed.xml file in the provided directory.
  void Write(std::filesystem::path out_dir);
  ~Atom() {}

 private:
  // Name of the website. This value is read from the sitewide config.
  std::string name_;

  // URL of the website. This value is read from the sitewide config.
  std::string homepage_url_;

  // URL of the feed.
  std::string feed_url_;

  // Name of the blog author. Currently, multiple author blogs and guest entries
  // are not supported.
  std::string author_;

  // Site update time. This is the timestamp of the moment the Write method is
  // called.
  time_t update_time_;

  // Ordered list of entries to write in the feed.
  std::vector<Entry> entries_;
};

}  // namespace yass
#endif /* ATOM_H */
