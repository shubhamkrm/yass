#ifndef CONFIG_H
#define CONFIG_H

#include <memory>
#include <string>
#include <vector>

namespace yass {

// Represents a navigation entry, consisting of a label and a URL. Multi-level
// navigation is not supported yet.
struct Navigation {
  // Navigation item label, such as 'Home', 'About' etc.
  std::string label;

  // URL for navigation item. It can be absolute or relative.
  std::string url;
};

// Maps values loaded from the top-level config.toml as a struct. This is used
// to set site-wide variables. All these variables are transparently available
// in the templates.
struct SiteConfig {
  // Name of the website.
  std::string name;

  // Base URL of the website. Relative URLs are converted to absolute using this
  // value.
  std::string base_url;

  // Owner name used in the copyright notice.
  std::string copyright_owner;

  // Theme name. This should be already installed at <project_root>/themes.
  std::string theme;

  // Single level navigation items.
  std::vector<Navigation> navigation;
};

// Reads top-level config.toml and returns a corresponding struct pointer.
std::shared_ptr<SiteConfig> ReadConfig(std::string_view config_path);

}  // namespace yass

#endif /* ifndef CONFIG_H */
