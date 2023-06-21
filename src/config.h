#ifndef CONFIG_H
#define CONFIG_H 

#include <memory>
#include <string>
#include <vector>

namespace yass {

struct Navigation {
    std::string label;
    std::string url;
};

struct SiteConfig {
    std::string name;
    std::string copyright_owner;
    std::vector<Navigation> navigation;
};
    
std::shared_ptr<SiteConfig> ReadConfig(std::string_view config_path); 

} /* yass  */ 

#endif /* ifndef CONFIG_H */
