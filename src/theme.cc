#include <chrono>
#include <ctime>
#include <filesystem>
#include <iostream>
#include <set>
#include <string>
#include <unordered_map>

#include "fileutils.h"
#include "processor.h"
#include "theme.h"
#include "vendor/kainjow/Mustache/mustache.hpp"

namespace yass {
namespace fs = std::filesystem;
using kainjow::mustache::data;
using kainjow::mustache::mustache;

namespace {

// Scans partials directory for files, and appends them to partials map.
std::unordered_map<std::string, data> AddPartials(std::string_view theme_name) {
  auto dir = fs::path("themes") / fs::path(theme_name) / fs::path("partials");
  std::unordered_map<std::string, data> partials;
  if (!fs::exists(dir) || !fs::is_directory(dir)) {
    std::cerr << "Partials directory not found. Skipping.\n";
    return partials;
  }
  for (const fs::directory_entry &entry : fs::directory_iterator(dir)) {
    if (!entry.is_regular_file()) {
      std::cerr << "Found non-regular file " << entry.path()
                << " in partials directory. Skipping.\n";
      continue;
    }
    std::string partial_name = entry.path().stem();
    std::function partial_provider = [entry]() {
      return ReadFile(entry.path());
    };
    partials[partial_name] = data{partial_provider};
  }
  return partials;
}
}  // namespace

Theme::Theme(std::shared_ptr<SiteConfig> site_config)
    : name_(site_config->theme),
      site_config_(site_config),
      partials_(AddPartials(name_)) {}

fs::path Theme::GetTemplatePath(const std::string page_type) const {
  return fs::path("themes") / fs::path(name_) / fs::path(page_type + ".tmpl");
}

std::string Theme::Render(
    Page *page,
    const std::set<PostSummary, PostSummaryCmp> post_summaries) const {
  fs::path path = GetTemplatePath(page->type);
  mustache tmpl(ReadFile(path));
  data template_values;
  template_values.set("sitename", site_config_->name);
  template_values.set("copyright_owner", site_config_->copyright_owner);

  data nav_values = data::type::list;
  for (auto &nav_entry : site_config_->navigation) {
    data nav_entry_value;
    nav_entry_value.set("text", nav_entry.label);
    nav_entry_value.set("url", site_config_->base_url + nav_entry.url);
    nav_values.push_back(nav_entry_value);
  }
  template_values.set("nav", nav_values);

  std::time_t current_time = std::time(0);
  template_values.set("update_date", std::ctime(&current_time));
  template_values.set("content", page->content);
  template_values.set("url", site_config_->base_url + page->path.string());
  template_values.set("prev_url", site_config_->base_url + page->prev.path);
  template_values.set("prev_label", page->prev.title);
  template_values.set("next_url", site_config_->base_url + page->next.path);
  template_values.set("next_label", page->next.title);

  for (auto &entry : page->metadata) {
    if (entry.second.type() == typeid(std::string))
      template_values.set(entry.first,
                          std::any_cast<std::string>(entry.second));
  }

  data summaries_data = data::type::list;
  if (page->type == "index") {
    for (auto &summary : post_summaries) {
      data summary_value;
      summary_value.set("date", summary.date);
      summary_value.set("title", summary.title);
      summary_value.set("url", site_config_->base_url + summary.path);
      summaries_data.push_back(summary_value);
    }
  }
  template_values.set("posts", summaries_data);

  for (auto &partial : partials_) {
    template_values.set(partial.first, partial.second);
  }
  return tmpl.render(template_values);
}
}  // namespace yass
