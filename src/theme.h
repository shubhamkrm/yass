#ifndef THEME_H
#define THEME_H

#include <filesystem>
#include <memory>
#include <string>

#include "config.h"
#include "parser.h"

namespace yass {
    
class Theme
{
private:
    std::string name;
    std::shared_ptr<SiteConfig> site_config;

public:
    Theme(std::string_view name, std::shared_ptr<SiteConfig> site_config);
    std::filesystem::path GetTemplatePath(std::string page_type) const;
    std::string Render(const Page *page) const;
    virtual ~Theme() {};
};

} /* yass  */ 
#endif /* THEME_H */
