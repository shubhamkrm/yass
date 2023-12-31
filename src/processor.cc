#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "fileutils.h"
#include "processor.h"

namespace yass {
namespace fs = std::filesystem;
const fs::path kContentDir = "./content/";
const fs::path kOutDir = "./static/";

namespace {

fs::path GetRelativePath(fs::path md_path) {
  return md_path.parent_path() / md_path.stem();
}

fs::path GetHtmlPath(fs::path relative_path) {
  return relative_path.replace_extension("html");
}

void WriteFile(const fs::path path, const std::string &content) {
  std::ofstream out(path);
  out << content;
}

time_t ParseDate(std::string date_string) {
  std::tm tm = {};
  std::stringstream ss(date_string);
  ss >> std::get_time(&tm, "%Y-%m-%d");
  return std::mktime(&tm);
}

}  // namespace

void Processor::ProcessAssets(std::vector<fs::path> assets) {
  for (auto &asset_path : assets) {
    fs::copy_file(kContentDir / asset_path, kOutDir / asset_path,
                  fs::copy_options::update_existing);
    std::cout << kOutDir / asset_path << std::endl;
  }
}

void Processor::ProcessPosts(std::vector<fs::path> posts) {
  std::vector<std::unique_ptr<Page>> pages;
  pages.reserve(posts.size());
  for (auto &post : posts) {
    std::string raw_post = ReadFile(kContentDir / (post));
    auto page = parser.Parse(raw_post);
    page->path = GetRelativePath(post);
    pages.push_back(std::move(page));
  }
  for (size_t i = 0; i < pages.size(); i++) {
    pages[i]->prev = i > 0 ? pages[i - 1]->GetPostSummary() : PostSummary();
    pages[i]->next =
        i < pages.size() - 1 ? pages[i + 1]->GetPostSummary() : PostSummary();
    auto &metadata = pages[i]->metadata;
    if (!metadata.contains("update_date")) {
      metadata["update_date"] = metadata["date"];
    }

    std::string processed_post = theme.Render(pages[i].get());
    fs::path out_path = kOutDir / GetHtmlPath(pages[i]->path);
    WriteFile(out_path, processed_post);
    std::cout << out_path << std::endl;

    auto summary = pages[i]->GetPostSummary();
    auto update_date = std::any_cast<std::string>(metadata["update_date"]);
    atom.AddEntry({.title = summary.title,
                   .url = pages[i]->path,
                   .published_time = ParseDate(summary.date),
                   .update_time = ParseDate(update_date),
                   .content = pages[i]->content});
    processed_posts_.insert({
        .path = pages[i]->path,
        .title = summary.title,
        .date = summary.date,
    });
  }
}

void Processor::ProcessPages(std::vector<fs::path> pages) {
  for (auto &page_path : pages) {
    std::string raw_post = ReadFile(kContentDir / page_path);
    auto page = parser.Parse(raw_post);
    page->path = GetRelativePath(page_path);
    std::string processed_post = theme.Render(page.get(), processed_posts_);
    fs::path out_path = kOutDir / GetHtmlPath(page->path);
    WriteFile(out_path, processed_post);
    std::cout << out_path << std::endl;
  }
}

void Processor::ProcessContent() {
  std::vector<fs::path> posts;
  std::vector<fs::path> pages;
  std::vector<fs::path> assets;

  fs::create_directory(kOutDir);

  for (const fs::directory_entry &dir_entry :
       fs::recursive_directory_iterator(kContentDir)) {
    fs::path relative_path = fs::relative(dir_entry, kContentDir);
    if (dir_entry.is_directory()) {
      fs::create_directory(kOutDir / relative_path);
    } else if (relative_path.extension() != ".md") {
      assets.push_back(relative_path);
    } else {
      if (relative_path.begin()->compare("posts") == 0) {
        posts.push_back(relative_path);

        // skip drafts
      } else if (relative_path.begin()->compare("drafts") != 0) {
        pages.push_back(relative_path);
      }
    }
  }
  std::cout << "Processing files... \n";
  ProcessAssets(assets);
  ProcessPosts(posts);
  ProcessPages(pages);
  atom.Write(kOutDir);

  std::string theme_path = theme.GetTemplatePath("post").parent_path();
  for (auto i = fs::recursive_directory_iterator(theme_path);
       i != fs::recursive_directory_iterator(); ++i) {
    // Skip partials directory
    if (i->path().filename() == "partials") {
      i.disable_recursion_pending();
      continue;
    }

    auto dir_entry = *i;
    fs::path relative_path = fs::relative(dir_entry, theme_path);
    if (dir_entry.is_directory()) {
      fs::create_directory(kOutDir / relative_path);
    }
    if (dir_entry.is_regular_file() && relative_path.extension() != ".tmpl") {
      fs::copy_file(
          dir_entry.path(), kOutDir / relative_path,
          fs::copy_options::recursive | fs::copy_options::update_existing);
    }
  }
}
}  // namespace yass
