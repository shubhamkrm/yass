#include "processor.h"

#include <fstream>
#include <iostream>
#include <filesystem>
#include <iterator>
#include <memory>
#include <sstream>
#include <string>
#include <vector>


namespace yass
{
namespace fs = std::filesystem;
const fs::path kContentDir = "./content/";
const fs::path kOutDir = "./static/";

namespace {

fs::path GetHtmlPath(fs::path md_path) {
    return md_path.parent_path() / (md_path.stem().string()+".html");
}

std::string ReadFile(const fs::path path) {
    std::ifstream in(path); 
    std::ostringstream sstr;
    sstr << in.rdbuf();
    return sstr.str();
} 

void WriteFile(const fs::path path, const std::string content) {
    std::ofstream out(path);
    out << content;
}

time_t ParseDate(std::string date_string) {
    std::tm tm = {};
    std::stringstream ss(date_string);
    ss >> std::get_time(&tm, "%Y-%m-%d");
    return std::mktime(&tm);
}

} // namespace
 
void Processor::ProcessAssets(std::vector<fs::path> assets) {
    for (auto &asset_path : assets) {
        fs::copy_file(kContentDir/asset_path, kOutDir/asset_path,
                fs::copy_options::update_existing);
        std::cout << kOutDir/asset_path << std::endl;
    }
}

void Processor::ProcessPosts(std::vector<fs::path> posts) {
    std::vector<std::unique_ptr<Page>> pages;
    pages.reserve(posts.size());
    for (auto &post: posts) {
        std::string raw_post = ReadFile(kContentDir/(post));
        auto page = parser.Parse(raw_post);
        page->path = GetHtmlPath(post);
        pages.push_back(std::move(page));
    }
    for (size_t i = 0; i < pages.size(); i++) {
        pages[i]->prev = i > 0 ?  pages[i - 1]->GetPostSummary() : PostSummary();
        pages[i]->next = i < pages.size() - 1 ? pages[i + 1]->GetPostSummary()
            : PostSummary();
        auto &metadata = pages[i]->metadata;
        if (!metadata.contains("update_date")) {
            metadata["update_date"] = metadata["date"];
        }
        
        std::string processed_post = theme.Render(pages[i].get());
        WriteFile(kOutDir/pages[i]->path, processed_post);
        std::cout << kOutDir/pages[i]->path << std::endl;

        atom.AddEntry({
                .title = metadata["title"],
                .url = pages[i]->path,
                .published_time = ParseDate(metadata["date"]),
                .update_time = ParseDate(metadata["update_date"]),
                .content = pages[i]->content
                });
        processed_posts_.push_back({
                .path =  pages[i]->path,
                .title = metadata["title"],
                .date =  metadata["date"]
                });
    }
}

void Processor::ProcessPages(std::vector<fs::path> pages) {
    for (auto &page_path : pages) {
        std::string raw_post = ReadFile(kContentDir/page_path);
        auto page = parser.Parse(raw_post);
        page->path = GetHtmlPath(page_path);
        std::string processed_post = theme.Render(page.get(), processed_posts_);
        WriteFile(kOutDir/page->path, processed_post);
        std::cout << kOutDir/page->path << std::endl;
    }
}

void Processor::ProcessContent()  {
    std::vector<fs::path> posts;
    std::vector<fs::path> pages;
    std::vector<fs::path> assets;

    fs::create_directory(kOutDir);

    for (const fs::directory_entry& dir_entry :
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
    for (const fs::directory_entry& dir_entry:
            fs::recursive_directory_iterator(theme_path)) {
        fs::path relative_path = fs::relative(dir_entry, theme_path);
        if (dir_entry.is_directory()) {
            fs::create_directory(kOutDir/relative_path);
        }
        if (dir_entry.is_regular_file() &&
                relative_path.extension() != ".tmpl") {
            fs::copy_file(dir_entry.path(), kOutDir/relative_path,
                    fs::copy_options::recursive | fs::copy_options::update_existing);
        }
    }
}
} /* yass */ 
