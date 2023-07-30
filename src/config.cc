#include <iostream>
#include <memory>
#include <string_view>

#include "config.h"

#define TOML_ENABLE_FORMATTERS 0
#include <toml++/toml.h>

namespace yass {
using ::toml::parse_result;

namespace {
void AssertStringConfigElement(const parse_result& config,
                               std::string_view element) {
  if (!config[element].is_string()) {
    std::cerr << "`" << element << "` is not available. I will die now.\n";
    exit(1);
  }
}
}  // namespace
std::shared_ptr<SiteConfig> ReadConfig(std::string_view config_path) {
  auto site_config = std::make_shared<SiteConfig>();
  const parse_result config = toml::parse_file(config_path);
  AssertStringConfigElement(config, "site_name");
  AssertStringConfigElement(config, "base_url");
  AssertStringConfigElement(config, "theme");

  site_config->name = config["site_name"].as_string()->value_or("");
  site_config->base_url = config["base_url"].as_string()->value_or("");
  site_config->theme = config["theme"].as_string()->value_or("");

  std::string copyright_owner;
  if (!config["copyright_owner"].is_string()) {
    std::cerr << "Copyright Owner is not available." << std::endl;
  } else {
    site_config->copyright_owner =
        config["copyright_owner"].as_string()->value_or("");
  }

  // TODO: Error handling
  if (const toml::array* nav_array = config["navigation"].as_array()) {
    nav_array->for_each([&site_config](auto&& el) {
      Navigation nav_entry = {
          .label = el.at_path("text").as_string()->value_or(""),
          .url = el.at_path("url").as_string()->value_or("")};
      site_config->navigation.push_back(nav_entry);
    });
  }
  return site_config;
}
}  // namespace yass
