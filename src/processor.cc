#include "processor.h"

#include <fstream>
#include <iostream>
#include <filesystem>
#include <memory>
#include <sstream>
#include <string>
#include <vector>


namespace yass
{
namespace fs = std::filesystem;
constexpr std::string_view kContentDir = "./content/";
constexpr std::string_view kOutDir = "./static/";

namespace {
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

} // namespace
 
void Processor::ProcessAssets(std::vector<fs::path> assets) {
    for (auto &asset_path : assets) {
        fs::copy_file(kContentDir/asset_path, kOutDir/asset_path,
                fs::copy_options::update_existing);
        std::cout << kOutDir/asset_path << std::endl;
    }
}

void Processor::ProcessPosts(std::vector<fs::path> posts) {
    for (auto &post : posts) {
        std::string raw_post = ReadFile(kContentDir/post);
        std::unique_ptr<Page> page = parser.Parse(raw_post);
        std::string processed_post = theme.Render(page.get());
        fs::path out_path = kOutDir/post.replace_extension(".html");
        WriteFile(out_path, processed_post);
        std::cout << out_path << std::endl;
        
        processed_posts_.push_back({
                .path = post,
                .title = page->metadata["title"],
                .date = page->metadata["date"]
                });
    }
}

void Processor::ProcessPages(std::vector<fs::path> pages) {
    for (auto &page_path : pages) {
        std::string raw_post = ReadFile(kContentDir/page_path);
        std::unique_ptr<Page> page = parser.Parse(raw_post);
        IndexPage *index_page = static_cast<IndexPage*>(page.get());
        std::string processed_post;
        if (index_page == nullptr) {
            processed_post = theme.Render(page.get());
        } else {
            index_page->post_summaries = processed_posts_;
            processed_post = theme.Render(index_page);
        }
        fs::path out_path = kOutDir/page_path.replace_extension(".html");
        WriteFile(out_path, processed_post);
        std::cout << out_path << std::endl;
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
