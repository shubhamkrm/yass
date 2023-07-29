#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <filesystem>
#include <set>
#include <string>

#include "atom.h"
#include "parser.h"
#include "structs.h"
#include "theme.h"

namespace yass {

// It is responsible for reading all the available inputs, such as themes,
// content files and static files, and produces a well-formed website structure
// which can be copied to a CDN or a web server.
class Processor {
 private:
  // A set of post summaries (consisting of title, path and date) for all
  // processed posts. This is used to create index pages. By convention, it only
  // considers pages in the `posts` directory.
  std::set<PostSummary, PostSummaryCmp> processed_posts_;

  // Parser dependency.
  Parser parser;

  // Theme processor dependency.
  Theme theme;

  // Atom feed processor dependency.
  Atom atom;

  // Processes static assets. Currently, it just copies the assets to the output
  // directory, but it can be augmented further to do more processing such as
  // asset size optimizations.
  void ProcessAssets(std::vector<std::filesystem::path> assets);

  // Processes pages. These are pages which are not in the posts directory.
  void ProcessPages(std::vector<std::filesystem::path> pages);

  // Processes posts. These are special pages which are in the posts directory.
  // These require special handling as these need to be indexed and linked to
  // each other.
  void ProcessPosts(std::vector<std::filesystem::path> posts);

 public:
  // Dependency injected constructor.
  Processor(Parser &parser, Theme &theme, Atom &atom)
      : parser(parser), theme(theme), atom(atom) {}

  // Reads all content files, theme files and static files and produces a final
  // website structure.
  void ProcessContent();

  // Default destructor.
  ~Processor() {}
};

}  // namespace yass
#endif /* PROCESSOR_H */
