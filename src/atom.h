#ifndef ATOM_H
#define ATOM_H

#include <filesystem>
#include <string>
#include <chrono>
#include <vector>

#include "config.h"

namespace yass {

class Atom
{
public:
    struct Entry {
        std::string title;
        std::string url;
        time_t      published_time;
        time_t      update_time;
        std::string content;
    };

    Atom(const SiteConfig& site_config);
    void AddEntry(const Entry& entry);
    void Write(std::filesystem::path out_dir);
    ~Atom() {}

private:
    std::string name_;
    std::string homepage_url_;
    std::string feed_url_;
    std::string author_;
    time_t      update_time_;
    std::vector<Entry> entries_;

};

} /* yass  */ 
#endif /* ATOM_H */
