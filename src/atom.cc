#include <chrono>
#include <ctime>
#include <filesystem>
#include "fmt/chrono.h"
#include <fstream>
#include <iostream>

#include "atom.h"
#include "vendor/kainjow/Mustache/mustache.hpp"

namespace yass {
using kainjow::mustache::data;
using kainjow::mustache::mustache;

constexpr char kTemplateString[] =
    R"atom(<?xml version="1.0" encoding="UTF-8"?>
<feed xmlns="http://www.w3.org/2005/Atom">
	<title>{{FEED_NAME}}</title>
	<id>{{HOMEPAGE_URL}}</id>
	<link rel="alternate" href="{{HOMEPAGE_URL}}"/>
	<link href="{{FEED_URL}}" rel="self"/>
	<updated>{{LAST_UPDATE_TIME}}</updated>
	<author>
		<name>{{AUTHOR_NAME}}</name>
	</author>
	{{#ENTRY}}
	<entry>
		<title>{{TITLE}}</title>
		<link rel="alternate" type="text/html" href="{{HTML_URL}}"/>
		<id>{{HTML_URL}}</id>
		<published>{{PUBLISHED_TIME}}</published>
		<updated>{{UPDATE_TIME}}</updated>
		<content type="html"><![CDATA[{{{CONTENT}}}]]></content>
	</entry>
	{{/ENTRY}}
</feed>
)atom";

constexpr char kDateFormat[] = "{0:%F}T{0:%T%Ez}";

namespace {
std::string GetTimeString(time_t &time) {
  return fmt::format(kDateFormat,
                     std::chrono::floor<std::chrono::seconds>(
                         std::chrono::system_clock::from_time_t(time)));
}
}  // namespace

Atom::Atom(const SiteConfig &site_config)
    : name_(site_config.name),
      homepage_url_(site_config.base_url),
      feed_url_(site_config.base_url + "feed.xml"),
      author_(site_config.copyright_owner),
      update_time_(std::time(0)),
      entries_() {}

void Atom::AddEntry(const Entry &entry) { entries_.push_back(entry); }

void Atom::Write(std::filesystem::path out_dir) {
  data atom;
  atom.set("FEED_NAME", name_);
  atom.set("HOMEPAGE_URL", homepage_url_);
  atom.set("FEED_URL", feed_url_);
  atom.set("AUTHOR_NAME", author_);
  atom.set("LAST_UPDATE_TIME", GetTimeString(update_time_));

  data entries = data::type::list;
  for (auto &entry : entries_) {
    data entry_data;
    entry_data.set("TITLE", entry.title);
    entry_data.set("HTML_URL", homepage_url_ + entry.url);
    entry_data.set("CONTENT", entry.content);
    entry_data.set("PUBLISHED_TIME", GetTimeString(entry.published_time));
    entry_data.set("UPDATE_TIME", GetTimeString(entry.update_time));
    entries.push_back(entry_data);
  }
  atom.set("ENTRY", entries);

  std::string output = mustache(kTemplateString).render(atom);
  std::ofstream out(out_dir / "feed.xml");
  out << output;
}

}  // namespace yass
