#ifndef THEME_H
#define THEME_H

#include <filesystem>
#include <memory>
#include <string>

#include "config.h"
#include "parser.h"
#include "structs.h"

namespace yass {
    
class Theme
{
private:
    std::string name;
    std::shared_ptr<SiteConfig> site_config;

public:
    Theme(std::string_view name, std::shared_ptr<SiteConfig> site_config);
    std::filesystem::path GetTemplatePath(std::string page_type) const;
    std::string Render(Page *page,
            const std::vector<PostSummary> post_summaries = {}) const;
    ~Theme() {};
};

} /* yass  */ 
#endif /* THEME_H */
