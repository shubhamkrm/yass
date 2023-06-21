#include "theme.h"
#include "ctemplate/template_dictionary.h"
#include "ctemplate/template_enums.h"

#include <chrono>
#include <ctime>
#include <filesystem>
#include <iostream>
#include <string>

#include <ctemplate/template.h>

namespace yass {
namespace fs = std::filesystem;

using ctemplate::TemplateDictionary;

Theme::Theme(std::string_view name, std::shared_ptr<SiteConfig> site_config) :
    name(name),
    site_config(site_config) {
}

fs::path Theme::GetTemplatePath(const std::string page_type) const {
    return fs::path("themes") / fs::path(name) / fs::path(page_type + ".tmpl");
}

std::string Theme::Render(const Page *page) const {
    fs::path path = GetTemplatePath(page->type);
    ctemplate::TemplateDictionary dict("page");

    dict.SetValue("sitename", site_config->name);
    dict.SetValue("copyright_owner", site_config->copyright_owner);
    for (auto &nav_entry: site_config->navigation) {
        TemplateDictionary *nav_dict = dict.AddSectionDictionary("nav");
        nav_dict->SetValue("text", nav_entry.label);
        nav_dict->SetValue("url", nav_entry.url);
    }

    std::time_t current_time = std::time(0);
    dict.SetValue("update_date", std::ctime(&current_time));
    dict.SetValue("content", page->content);
    for (auto &entry: page->metadata) {
        dict.SetValue(entry.first, entry.second);
    }
    if (page->type == "index") {
        const IndexPage *index_page = static_cast<const IndexPage*>(page);
        for (auto &summary: index_page->post_summaries) {
            TemplateDictionary *posts_dict = dict.AddSectionDictionary("posts");
            posts_dict->SetValue("date", summary.date);
            posts_dict->SetValue("title", summary.title);
            posts_dict->SetValue("url", summary.path);
        }
    }
    std::string output;
    ctemplate::ExpandTemplate(path.string(),
            ctemplate::STRIP_WHITESPACE, &dict, &output);
    return output;
}
} /* yass  */ 
