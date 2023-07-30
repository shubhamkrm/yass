#ifndef THEME_H
#define THEME_H

#include <filesystem>
#include <memory>
#include <set>
#include <string>
#include <unordered_map>

#include "config.h"
#include "parser.h"
#include "structs.h"
#include "vendor/kainjow/Mustache/mustache.hpp"

namespace yass {

// Processes and hydrates the chosen theme with content. It assumes the
// following conventions are followed:
// 1. All page types have a corresponding template file. For example,
//    `post.tmpl` will be used to render pages of type `post`.
// 2. Themes reside at <project_root>/themes/<theme_name>
// 3. Partials reside at <theme_dir>/partials/
// 4. All other files other than tmpl files would be copied to the output folder
//    with the same directory structure.
class Theme {
 private:
  // Name of the chosen theme.
  std::string name_;

  // Site config parsed from the config.toml file.
  std::shared_ptr<SiteConfig> site_config_;

  // Stores mapping for all partials at init time.
  std::unordered_map<std::string, kainjow::mustache::data> partials_;

 public:
  // Initialized a theme with the name and site config.
  Theme(std::string_view name, std::shared_ptr<SiteConfig> site_config);

  // Returns a template path for a given theme name. This assumes that the
  // convention of storing themes at `theme/<theme-name>/<page_type>.tmpl` is
  // being followed.
  std::filesystem::path GetTemplatePath(std::string page_type) const;

  // Renders the theme by replacing placeholders with the actual values and
  // returns the final page as a string. It accepts a Page struct as the value
  // provider and an optional post summaries set to produce index if required.
  std::string Render(
      Page *page,
      const std::set<PostSummary, PostSummaryCmp> post_summaries = {}) const;
  ~Theme(){};
};

}  // namespace yass
#endif /* THEME_H */
