#include <ctemplate/template.h>

#include <chrono>
#include <ctime>
#include <filesystem>
#include <iostream>
#include <set>
#include <string>

#include "processor.h"
#include "theme.h"

namespace yass {
namespace fs = std::filesystem;

using ctemplate::TemplateDictionary;

Theme::Theme(std::string_view name, std::shared_ptr<SiteConfig> site_config)
    : name_(name), site_config_(site_config) {}

fs::path Theme::GetTemplatePath(const std::string page_type) const {
  return fs::path("themes") / fs::path(name_) / fs::path(page_type + ".tmpl");
}

std::string Theme::Render(
    Page *page,
    const std::set<PostSummary, PostSummaryCmp> post_summaries) const {
  fs::path path = GetTemplatePath(page->type);
  ctemplate::TemplateDictionary dict("page");

  dict.SetValue("sitename", site_config_->name);
  dict.SetValue("copyright_owner", site_config_->copyright_owner);
  for (auto &nav_entry : site_config_->navigation) {
    TemplateDictionary *nav_dict = dict.AddSectionDictionary("nav");
    nav_dict->SetValue("text", nav_entry.label);
    nav_dict->SetValue("url", site_config_->base_url + nav_entry.url);
  }

  std::time_t current_time = std::time(0);
  dict.SetValue("update_date", std::ctime(&current_time));
  dict.SetValue("content", page->content);
  dict.SetValue("url", site_config_->base_url + page->path.string());

  dict.SetValue("prev_url", site_config_->base_url + page->prev.path);
  dict.SetValue("prev_label", page->prev.title);
  if (!page->prev.path.empty() && !page->prev.title.empty()) {
    dict.ShowSection("prev_post");
  }
  dict.SetValue("next_url", site_config_->base_url + page->next.path);
  dict.SetValue("next_label", page->next.title);
  if (!page->next.path.empty() && !page->next.title.empty()) {
    dict.ShowSection("next_post");
  }
  for (auto &entry : page->metadata) {
    if (entry.second.type() == typeid(std::string))
      dict.SetValue(entry.first, std::any_cast<std::string>(entry.second));
  }
  if (page->type == "index") {
    for (auto &summary : post_summaries) {
      TemplateDictionary *posts_dict = dict.AddSectionDictionary("posts");
      posts_dict->SetValue("date", summary.date);
      posts_dict->SetValue("title", summary.title);
      posts_dict->SetValue("url", site_config_->base_url + summary.path);
    }
  }
  std::string output;
  ctemplate::ExpandTemplate(path.string(), ctemplate::STRIP_WHITESPACE, &dict,
                            &output);
  return output;
}
}  // namespace yass
