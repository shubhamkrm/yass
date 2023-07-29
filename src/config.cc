#include <iostream>
#include <memory>

#include "config.h"

#define TOML_ENABLE_FORMATTERS 0
#include <toml++/toml.h>

namespace yass {
std::shared_ptr<SiteConfig> ReadConfig(std::string_view config_path) {
  auto site_config = std::make_shared<SiteConfig>();
  const auto config = toml::parse_file(config_path);
  if (!config["site_name"].is_string()) {
    std::cerr << "Site Name is not available. I will die now." << std::endl;
    exit(1);
  }
  if (!config["base_url"].is_string()) {
    std::cerr << "Base URL is not available. I will die now." << std::endl;
    exit(1);
  }
  site_config->name = config["site_name"].as_string()->value_or("");
  site_config->base_url = config["base_url"].as_string()->value_or("");
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
